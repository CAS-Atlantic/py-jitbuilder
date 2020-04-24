"""
A module for generating the JitBuilder Python client API.
"""

import os
import re
import datetime
import shutil
import argparse
import cppgen
import keyword
import textwrap
from genutils import APIDescription, PrettyPrinter, list_str_prepend
from collections import Counter


class PyGenerator(cppgen.CppGenerator):
    def write_includes(self, writer):
        includes = """\
            #include <pybind11/pybind11.h>
            #include <pybind11/functional.h>
            #include <cstring>
            #include <iostream>
            #include "JitBuilder.hpp"
            
            namespace py = pybind11;\n"""
        writer.write(textwrap.dedent(includes))

    def write_module(self, writer, api_desc):
        self.write_includes(writer)
        writer.write("\n")

        for c in api_desc.classes():
            if c.callbacks():
                writer.write(
                    "class Py{} : public OMR::JitBuilder::{} {{\n".format(
                        c.name(), self.get_class_name(c)
                    )
                )
                writer.indent()
                writer.write("public:\n")
                writer.write("using {cn}::{cn};\n".format(cn=c.name()))
                for callback in c.callbacks():
                    self.write_virtual_wrapper(writer, c, callback)

                # write parent class virtual funcs
                # if c.has_parent():
                #     for callback in c.parent().callbacks():
                #         self.write_virtual_wrapper(writer, c, callback)

                writer.outdent()
                writer.write("};\n")

        self.write_compile_method_builder_wrapper(writer)

        writer.write("\n")

        writer.write("PYBIND11_MODULE(jitbuilder, m) {\n")
        writer.indent()

        self.write_top_level_services(writer, api_desc)

        non_parents = []
        for c in api_desc.classes():
            if c.has_parent():
                non_parents.append(c)
            else:
                self.write_class_def(writer, c)
        for c in non_parents:
            self.write_class_def(writer, c)

        writer.outdent()
        writer.write("}\n\n")

    def write_virtual_wrapper(self, writer, class_desc, callback):
        ret_type = self.get_client_type(callback.return_type())
        fn = callback.name()
        writer.write(
            "{ret_type} {fn}({parms}) override {{\n".format(
                ret_type=ret_type,
                fn=fn,
                parms=self.generate_parm_list(
                    callback.parameters(), namespace="OMR::JitBuilder::"
                ),
            )
        )
        writer.indent()
        writer.write("PYBIND11_OVERLOAD(\n")
        writer.indent()
        writer.write("{ret_type},\n".format(ret_type=ret_type))
        writer.write("OMR::JitBuilder::{},\n".format(self.get_class_name(class_desc)))
        writer.write("{fn},\n".format(fn=fn))
        writer.write("{}".format(", ".join([p.name() for p in callback.parameters()])))
        writer.outdent()
        writer.write(");\n")
        writer.outdent()
        writer.write("}\n")

    def write_top_level_services(self, writer, api_desc):
        for service in api_desc.services():
            name = service.name()
            if "ppointer" not in [p.type().name() for p in service.parameters()]:
                writer.write(
                    'm.def("{}", &{});\n'.format(self.to_snake_case(name), name)
                )
            else:
                writer.write(
                    'm.def("{}", &{}_wrapper);\n'.format(self.to_snake_case(name), name)
                )
        writer.write("\n")

    def write_compile_method_builder_wrapper(self, writer):
        writer.write(
            "void * compileMethodBuilder_wrapper(OMR::JitBuilder::MethodBuilder mb) {"
        )
        writer.indent()
        writer.write(
            textwrap.dedent(
                """ 
            void * compiledMethod = 0;
            auto rc = compileMethodBuilder(&mb, &compiledMethod);
            
            if (rc != 0)
            {
                std::cerr << "FAIL: compilation error " << rc << "\\n";
                exit(-2);
            }

            return compiledMethod;\n"""
            )
        )
        writer.outdent()
        writer.write("}")

    def write_class_def(self, writer, class_desc):
        prefix = ", Py{}".format(class_desc.name()) if class_desc.callbacks() else ""
        if class_desc.has_parent():
            writer.write(
                'py::class_<OMR::JitBuilder::{}{}, OMR::JitBuilder::{}>(m, "{}")\n'.format(
                    self.get_class_name(class_desc),
                    prefix,
                    class_desc.parent().name(),
                    class_desc.name(),
                )
            )
        else:
            writer.write(
                'py::class_<OMR::JitBuilder::{}{}>(m, "{}")\n'.format(
                    self.get_class_name(class_desc), prefix, class_desc.name()
                )
            )
        writer.indent()

        for field in class_desc.fields():
            self.write_field(writer, field, class_desc)

        # write needed impl field
        if not class_desc.has_parent():
            writer.write(
                '.def_readwrite("impl", &OMR::JitBuilder::{}::_impl)\n'.format(
                    self.get_class_name(class_desc)
                )
            )

        # self.write_impl_getter_impl(writer, class_desc)

        for ctor in class_desc.constructors():
            self.write_ctor(writer, ctor)

        # write impl constructor
        writer.write(".def(py::init<void *>())\n")

        # write impl init service delcaration
        # writer.write(
        #     '.def("initialize_from_impl", &OMR::JitBuilder::{}::initializeFromImpl)\n'.format(
        #         class_desc.name()
        #     )
        # )

        for callback in class_desc.callbacks():
            writer.write(
                '.def("{}", &OMR::JitBuilder::{}::{})\n'.format(
                    self.to_snake_case(callback.name()),
                    class_desc.name(),
                    callback.name(),
                )
            )

        # Hacky and should prolly be added to the API description
        counts = Counter([s.name() for s in class_desc.services()])
        for service in class_desc.services():
            if counts[service.name()] > 1 or service.is_vararg():
                if self.needs_wrapper(service):
                    self.write_lambda_wrapper(writer, service)
                else:
                    writer.write(
                        '.def("{}", py::overload_cast<{}>(&OMR::JitBuilder::{}::{}))\n'.format(
                            self.to_snake_case(service.name()),
                            self.generate_parm_list_types(
                                service.parameters(), namespace="OMR::JitBuilder::"
                            ),
                            class_desc.name(),
                            service.name(),
                        )
                    )
            else:
                if self.needs_wrapper(service):
                    self.write_lambda_wrapper(writer, service)
                else:
                    writer.write(
                        '.def("{}", &OMR::JitBuilder::{}::{})\n'.format(
                            self.to_snake_case(service.name()),
                            class_desc.name(),
                            service.name(),
                        )
                    )

        # write allocator
        self.write_allocator(writer, class_desc)

        writer.write("\n")

        for c in class_desc.inner_classes():
            writer.outdent()
            self.write_class_def(writer, c)
            writer.indent()

        writer.outdent()

    def needs_wrapper(self, service):
        for p in service.parameters():
            if (
                p.type().name() == "ppointer"
                or p.is_array()
                or p.is_in_out()
                or p.type().name() == "constString"
                or p.type().name() == "string"
            ):
                return True
        return False

    def write_lambda_wrapper(self, writer, service):
        fn = self.to_snake_case(service.name())
        writer.write('.def("{}",\n'.format(fn))
        writer.indent()
        receiver = "OMR::JitBuilder::{} * self{}".format(
            service.owning_class().name(), ", " if service.parameters() else ""
        )
        writer.write(
            "[] ({}{}) {{\n".format(
                receiver,
                self.generate_parm_list_wrapper(
                    service.parameters(), namespace="OMR::JitBuilder::"
                ),
            )
        )
        writer.indent()
        args = []
        rets = []
        for p in service.parameters():
            if p.type().name() == "constString" or p.type().name() == "string":
                name = "{}_wrapper".format(p.name())
                writer.write(
                    "char * {} = new char [strlen({})];\n".format(name, p.name())
                )
                writer.write("strcpy({}, {});\n\n".format(name, p.name()))
                args.append("{}".format(name))
            elif p.type().name() == "ppointer" or p.is_in_out():
                writer.write(
                    "{}_wrapper = &{};\n".format(
                        self.generate_parm(p, namespace="OMR::JitBuilder::"), p.name()
                    )
                )
                args.append("{}_wrapper".format(p.name()))
            elif p.is_array():
                args.append("{}.data()".format(p.name()))
            else:
                args.append(p.name())

            if p.is_in_out():
                rets.append(p.name())

        if service.return_type().name() != "none":
            ret = "self->{fn}({args})".format(fn=service.name(), args=", ".join(args))
            rets.insert(0, ret)
        else:
            writer.write(
                "self->{fn}({args});\n".format(fn=service.name(), args=", ".join(args))
            )
        if rets:
            ret_stmt = (
                "return py::make_tuple({});\n" if len(rets) > 1 else "return {};\n"
            )
            writer.write(ret_stmt.format(", ".join(rets)))

        # for p in service.parameters():
        #     if p.type().name() == "ppointer" or p.is_in_out() or p.is_array():
        #         writer.write("{name} = * {name}_wrapper;\n".format(name=p.name()))
        writer.outdent()
        writer.write("}\n")
        writer.outdent()
        writer.write(")\n")

    def generate_wrapper_return(self, service):
        return ""

    def generate_parm_wrapper(self, parm_desc, namespace="", is_client=True):
        """
        Produces a parameter declaration from a given parameter description.
        The parameter declaration is usable in a function declaration.
        """
        fmt = ""
        if parm_desc.is_in_out():
            fmt = "{t} {n}"
        elif parm_desc.is_array():
            fmt = "std::vector<{t}> {n}"
        else:
            fmt = "{t} {n}"
        t = parm_desc.type()
        t = self.get_client_type(t, namespace) if is_client else self.get_impl_type(t)
        return fmt.format(t=t, n=parm_desc.name())

    def generate_parm_list_wrapper(self, parms_desc, namespace="", is_client=True):
        """
        Produces a stringified comma seperated list of parameter
        declarations from a list of parameter descriptions. The list
        is usable as the parameter listing of a function declaration.
        """
        return ", ".join(
            [
                self.generate_parm_wrapper(p, namespace=namespace, is_client=is_client)
                for p in parms_desc
            ]
        )

    def ppointer_to_pointer(self, type_name):
        return type_name.replace("*", "", 1)

    def write_allocator(self, writer, class_desc):
        # for c in class_desc.inner_classes():
        # self.write_allocator(writer, c)
        name = self.get_allocator_name(class_desc)
        writer.write(
            '.def("{}", &OMR::JitBuilder::{});\n'.format(self.to_snake_case(name), name)
        )

    def write_impl_getter_impl(self, writer, class_desc):
        fn = self.impl_getter_name(class_desc)
        writer.write(
            '.def("get_impl", &OMR::JitBuilder::{cn}::{fn})\n'.format(
                cn=self.get_class_name(class_desc), fn=fn
            )
        )

    def write_field(self, writer, field, class_desc):
        writer.write(
            '.def_readwrite("{}", &OMR::JitBuilder::{}::{})\n'.format(
                self.to_snake_case(field.name()),
                self.get_class_name(class_desc),
                field.name(),
            )
        )

        """
        
        # write needed impl field
        if not class_desc.has_parent():
            writer.write("public: void* _impl;\n")

        for ctor in class_desc.constructors():
            decls = self.generate_ctor_decl(ctor, name)
            writer.write(decls)

        # write impl constructor
        writer.write("public: explicit {name}(void * impl);\n".format(name=name))

        # write impl init service delcaration
        writer.write("protected: void initializeFromImpl(void * impl);\n")

        dtor_decl = self.generate_dtor_decl(class_desc)
        writer.write(dtor_decl)

        for callback in class_desc.callbacks():
            decl = self.generate_class_service_decl(callback, is_callback=True)
            writer.write(decl)

        for service in class_desc.services():
            decl = self.generate_class_service_decl(service)
            writer.write(decl)

        if has_extras:
            writer.write(self.generate_include('{}ExtrasInsideClass.hpp'.format(class_desc.name())))


        writer.outdent()
        writer.write('};\n')
        # write callback thunk definitions
        for callback in class_desc.callbacks():
            self.write_callback_thunk(writer, class_desc, callback)

        self.write_impl_ctor_impl(writer, class_desc)
        writer.write("\n")

        # write class initializer (called from all constructors)
        self.write_impl_initializer(writer, class_desc)
        writer.write("\n")

        # write buildIl callback decl
        if class_desc.has_parent() and class_desc.parent().name() == "IlBuilder":
            self.write_buildil_callback_impl(writer, class_desc)
            writer.write("\n")

        # write service definitions
        for s in class_desc.services():
            self.write_class_service_impl(writer, s, class_desc)
            writer.write("\n")

        # write callback definitions
        for s in class_desc.callbacks():
            self.write_class_service_impl(writer, s, class_desc)
            writer.write("\n")

        self.write_allocator_impl(writer, class_desc)
        writer.write("\n")

        # write buildIl callback decl
        if class_desc.has_parent() and class_desc.parent().name() == "IlBuilder":
            self.write_buildil_callback_impl(writer, class_desc)

        # write service definitions
        for s in class_desc.services():
            self.write_class_service_impl(writer, s, class_desc)
            writer.write("\n")

        # write service definitions
        for s in class_desc.callbacks():
            self.write_class_service_impl(writer, s, class_desc)
            writer.write("\n")

        self.write_allocator_impl(writer, class_desc)
        writer.write("\n")
        """

        # writer.outdent()
        # writer.write("\n")
        """
"""

    def write_ctor(self, writer, ctor_desc):
        if self.needs_wrapper(ctor_desc):
            self.write_ctor_wrapper(writer, ctor_desc)
        else:
            writer.write(
                ".def(py::init<{}>())\n".format(
                    self.generate_parm_list_types(
                        ctor_desc.parameters(), namespace="OMR::JitBuilder::"
                    )
                )
            )

    """
    def write_lambda_wrapper(self, writer, service):
        fn = self.to_snake_case(service.name())
        writer.write('.def("{}",\n'.format(fn))
        writer.indent()
        receiver = "OMR::JitBuilder::{} * self{}".format(
            service.owning_class().name(), ", " if service.parameters() else ""
        )
        writer.write(
            "[] ({}{}) {{\n".format(
                receiver,
                self.generate_parm_list_wrapper(
                    service.parameters(), namespace="OMR::JitBuilder::"
                ),
            )
        )
        writer.indent()
        args = []
        for p in service.parameters():
            if p.type().name() == "constString":
                name = "{}_wrapper".format(p.name())
                writer.write(
                    "char * {} = new char [strlen({})];\n".format(name, p.name())
                )
                writer.write("strcpy({},{});\n\n".format(name, p.name()))
                args.append("{}".format(name))
            elif p.type().name() == "ppointer" or p.is_in_out() or p.is_array():
                writer.write(
                    "{}_wrapper = &{};\n".format(
                        self.generate_parm(p, namespace="OMR::JitBuilder::"), p.name()
                    )
                )
                args.append("{}_wrapper".format(p.name()))
            else:
                args.append(p.name())
        ret = "return " if service.return_type().name() != "none" else ""
        writer.write(
            "{ret}self->{fn}({args});\n".format(
                ret=ret, fn=service.name(), args=", ".join(args)
            )
        )

        for p in service.parameters():
            if p.type().name() == "ppointer" or p.is_in_out() or p.is_array():
                writer.write("{name} = * {name}_wrapper;\n".format(name=p.name()))
        writer.outdent()
        writer.write("}\n")
        writer.outdent()
        writer.write(")\n")
    """

    def write_ctor_wrapper(self, writer, ctor_desc):
        writer.write(".def(py::init(\n".format())
        writer.indent()
        writer.write(
            "[] ({}) {{\n".format(
                self.generate_parm_list_wrapper(
                    ctor_desc.parameters(), namespace="OMR::JitBuilder::"
                )
            )
        )
        writer.indent()

        args = []
        for p in ctor_desc.parameters():
            if p.type().name() == "constString" or p.type().name() == "string":
                name = "{}_wrapper".format(p.name())
                writer.write(
                    "char * {} = new char [strlen({})];\n".format(name, p.name())
                )
                writer.write("strcpy({}, {});\n".format(name, p.name()))
                args.append("{}".format(name))
            elif p.type().name() == "ppointer" or p.is_in_out():
                writer.write(
                    "{}_wrapper = &{};\n".format(
                        self.generate_parm(p, namespace="OMR::JitBuilder::"), p.name()
                    )
                )
                args.append("{}_wrapper".format(p.name()))
            elif p.is_array():
                args.append("{}.data()".format(p.name()))
            else:
                args.append(p.name())

        writer.write("\n")
        writer.write(
            "return OMR::JitBuilder::{fn}({args});\n".format(
                fn=ctor_desc.name(), args=", ".join(args)
            )
        )
        # for p in ctor_desc.parameters():
        #     if p.type().name() == "ppointer" or p.is_in_out() or p.is_array():
        #         writer.write("{name} = * {name}_wrapper;\n".format(name=p.name()))

        # writer.write("return ret;\n")
        writer.outdent()
        writer.write("}\n")
        writer.outdent()
        writer.write("))\n")

    # def write_top_level_funcs(self, writer, api_desc)

    def __init__(self, api, headerdir, extras=[]):
        cppgen.CppGenerator.__init__(self, api, headerdir, extras)

    #     self.impl_include_files = self.gen_api_impl_includes(api.classes())

    # def gen_api_impl_includes(self, classes_desc, api_headers_dir=None):
    #     return [c.name() for c in classes_desc]

    # utilities
    def generate_parm_list_types(self, parms_desc, namespace="", is_client=True):
        """
        Produces a stringified comma seperated list of parameter
        declarations from a list of parameter descriptions. The list
        is usable as the parameter listing of a function declaration.
        """
        return ", ".join(
            [
                self.generate_parm_types(p, namespace=namespace, is_client=is_client)
                for p in parms_desc
            ]
        )

    def generate_parm_types(self, parm_desc, namespace="", is_client=True):
        """
        Produces a parameter declaration from a given parameter description.
        The parameter declaration is usable in a function declaration.
        """
        fmt = "{t}*" if parm_desc.is_in_out() or parm_desc.is_array() else "{t}"
        t = parm_desc.type()
        t = self.get_client_type(t, namespace) if is_client else self.get_impl_type(t)
        return fmt.format(t=t)

    def to_snake_case(self, word):
        patterns = [(r"(.)([0-9]+)", r"\1_\2"), (r"([a-z]+)([A-Z])", r"\1_\2")]
        engines = [
            (pattern, replacement, re.compile(pattern))
            for pattern, replacement in patterns
        ]
        for data in engines:
            pattern, replacement, eng = data
            word = eng.sub(replacement, word)
        word = word.lower()
        word = word if not keyword.iskeyword(word) else "{}_".format(word)
        return word


# main generator ########

if __name__ == "__main__":
    default_dest = os.path.join(os.getcwd(), "client")
    default_lib_path = r"../../lib/jitbuilder.lib"
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--sourcedir",
        type=str,
        default=default_dest,
        help="destination directory for the generated source files",
    )

    parser.add_argument(
        "--libdir", type=str, default=default_lib_path, help="jitbuilder library path"
    )

    parser.add_argument("description", help="path to the API description file")
    args = parser.parse_args()

    with open(args.description) as api_src:
        api_description = APIDescription.load_json_file(api_src)

    generator = PyGenerator(api_description, "")

    namespaces = api_description.namespaces()
    class_names = api_description.get_class_names()

    with open(os.path.join(args.sourcedir, "jitbuilder_bindings.cpp"), "w") as writer:
        generator.write_module(PrettyPrinter(writer), api_description)

