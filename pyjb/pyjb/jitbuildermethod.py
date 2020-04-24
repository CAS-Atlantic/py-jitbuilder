import jitbuilder
import ctypes
import sys
import inspect
from pyjb.pyjbtypes import *
from pyjb import asttyper
from pyjb import typedast
import numpy as np


def mangle_name(name, signature):
    return f"{name}{str(hash(tuple(signature)))}"


class JitBuilderMethod(jitbuilder.MethodBuilder):
    def __init__(self, return_type, func_name, params, td, ast):
        super().__init__(td)
        self.locals = []
        self.arrays = []
        self.return_type = return_type
        self.params = params
        self.ast = ast
        self.td = td

        # self.define_name(mangle_name(func_name, params))
        self.define_name(func_name)

        for arg_name, arg_type in params.items():
            if is_array(arg_type):
                self.define_array_parameter(arg_name, to_jitbuilder_type(arg_type))
                self.arrays.append(arg_name)
            else:
                self.define_parameter(arg_name, to_jitbuilder_type(arg_type))
        self.define_return_type(to_jitbuilder_type(return_type))

    def buildIL(self):
        self.visit(self.ast)
        if self.return_type == "void":
            self.return_()
        return True

    def visit(self, node, builder=None):
        name = f"visit_{type(node).__name__}"
        if hasattr(self, name):
            return getattr(self, name)(node, builder)
        else:
            return self.generic_visit(node)

    def generic_visit(self, node):
        raise NotImplementedError(type(node).__name__)

    def visit_Int(self, node, builder):
        if builder:
            _self = builder
        else:
            _self = self
        return _self.const_int_64(node.n)

    def visit_Float(self, node, builder):
        if builder:
            _self = builder
        else:
            _self = self
        return _self.const_double(node.n)

    def visit_Return(self, node, builder):
        if builder:
            _self = builder
        else:
            _self = self
        val = self.visit(node.value, builder)
        _self.return_(val)

    def visit_If(self, node, builder):
        if builder:
            _self = builder
        else:
            _self = self

        compare = self.visit(node.test, builder)
        conditional_builder_true, conditional_builder_false = self.if_then_else(
            None, None, compare
        )

        for statement in node.body:
            self.visit(statement, conditional_builder_true)

        for statement in node.orelse:
            self.visit(statement, conditional_builder_false)

    def visit_Compare(self, node, builder):
        if builder:
            _self = builder
        else:
            _self = self
        comparator = self.visit(node.comparator, builder)
        left = self.visit(node.left, builder)
        if isinstance(node.op, typedast.Eq):
            return _self.equal_to(left, comparator)
        if isinstance(node.op, typedast.Lt):
            return _self.less_than(left, comparator)
        if isinstance(node.op, typedast.Gt):
            return _self.greater_than(left, comparator)
        else:
            raise NotImplementedError

    def visit_BoolOp(self, node, builder):
        if builder:
            _self = builder
        else:
            _self = self
        if isinstance(node.op, typedast.Or):
            if len(node.values) == 2:
                return _self.or_(
                    self.visit(node.values[0], builder),
                    self.visit(node.values[1], builder),
                )
            else:
                temp = None
                for i in range(1, len(node.values)):
                    if temp:
                        temp = _self.or_(temp, self.visit(node.values[i], builder))
                    else:
                        temp = _self.or_(
                            self.visit(node.values[0], builder),
                            self.visit(node.values[1], builder),
                        )
                return temp

        elif isinstance(node.op, typedast.And):
            if len(node.values) == 2:
                return _self.and_(
                    self.visit(node.values[0], builder),
                    self.visit(node.values[1], builder),
                )
            else:
                temp = None
                for i in range(1, len(node.values)):
                    if temp:
                        temp = _self.and_(temp, self.visit(node.values[i], builder))
                    else:
                        temp = _self.and_(
                            self.visit(node.values[0], builder),
                            self.visit(node.values[1], builder),
                        )
                return temp

    def visit_Call(self, node, builder):
        if builder:
            _self = builder
        else:
            _self = self
        args = []
        for arg in node.args:
            args.append(self.visit(arg, builder))
        return _self.call(node.fn.id, len(node.args), args)

    def visit_While(self, node, builder):
        if builder:
            _self = builder
        else:
            _self = self
        _self.store("iterateUntil", self.visit(node.test, builder))
        loop_builder = _self.while_do_loop("iterateUntil", None)
        for statement in node.body:
            self.visit(statement, loop_builder)
        loop_builder.store("iterateUntil", self.visit(node.test, loop_builder))

    def visit_For(self, node, builder):
        if builder:
            _self = builder
        else:
            _self = self
        begin = self.visit(node.begin, builder)
        end = self.visit(node.end, builder)
        step = self.visit(node.step, builder)

        loop_builder = _self.for_loop_up(node.target.id, None, begin, end, step)
        for statement in node.body:
            self.visit(statement, loop_builder)

    def visit_Assign(self, node, builder):
        if builder:
            _self = builder
        else:
            _self = self
        value = self.visit(node.value, builder)
        var = node.target
        if isinstance(var, str):
            if var not in self.locals and var not in self.arrays:
                self.define_local(var, to_jitbuilder_type(node.type))
                self.locals.append(var)
            _self.store(var, value)
        elif isinstance(var, typedast.Index):
            var = self.visit(node.target, builder)
            _self.store_at(var, value)
        else:
            var = var.id
            _self.store(var, value)

    def visit_Prim(self, node, builder):
        if builder:
            _self = builder
        else:
            _self = self
        if node.fn == "add":
            a = self.visit(node.args[0], builder)
            b = self.visit(node.args[1], builder)
            return _self.add(a, b)
        elif node.fn == "sub":
            a = self.visit(node.args[0], builder)
            b = self.visit(node.args[1], builder)
            return _self.sub(a, b)
        elif node.fn == "mult":
            a = self.visit(node.args[0], builder)
            b = self.visit(node.args[1], builder)
            return _self.mul(a, b)
        elif node.fn == "div":
            a = self.visit(node.args[0], builder)
            b = self.visit(node.args[1], builder)
            return _self.div(a, b)
        else:
            raise NotImplementedError

    def visit_NoOp(self, node):
        pass

    def visit_Var(self, node, builder):
        if builder:
            _self = builder
        else:
            _self = self
        if isinstance(node.id, str):
            return _self.load(node.id)
        elif isinstance(node.id, typedast.Index):
            return self.visit(node.id, builder)

    def visit_FunctionDef(self, node, builder):
        list(map(self.visit, node.body))

    def visit_Index(self, node, builder, ndim=1, ctx="load"):
        if builder:
            _self = builder
        else:
            _self = self
        if node.ctx == "store":
            if isinstance(node.value, typedast.Var):
                value = self.visit(node.value, builder)
                idx = self.visit(node.idx, builder)
                return _self.index_at(to_jitbuilder_type(node.value.type), value, idx)
            else:
                arr = _self.convert_to(
                    to_jitbuilder_type(node.value.value.type),
                    self.visit_Index(node.value, builder, ndim=2, ctx="store"),
                )
                idx = self.visit(node.idx, builder)
                return _self.index_at(
                    to_jitbuilder_type(node.value.value.type), arr, idx
                )
        if isinstance(node.value, typedast.Var):
            value = self.visit(node.value, builder)
            idx = self.visit(node.idx, builder)
            return (
                _self.load_at(
                    to_jitbuilder_type(node.value.type),
                    _self.index_at(to_jitbuilder_type(node.value.type), value, idx),
                )
                if ndim == 1
                else _self.load_at(
                    self.td.p_address, _self.index_at(self.td.p_address, value, idx)
                )
            )
        else:
            arr = _self.convert_to(
                to_jitbuilder_type(node.value.value.type),
                self.visit_Index(node.value, builder, ndim=2),
            )
            idx = self.visit(node.idx, builder)
            return _self.load_at(
                to_jitbuilder_type(node.value.value.type),
                _self.index_at(to_jitbuilder_type(node.value.value.type), arr, idx),
            )


def wrap_module(fptr, return_type, args):
    typed_func = wrap_function(fptr, return_type, args)
    dispatch = dispatcher(typed_func)
    return dispatch


def dispatcher(fn):
    fn.__name__ = "jitbuilder_func"

    def _call_closure(*args):
        cargs = list(fn._argtypes_)
        pargs = list(args)
        rargs = map(wrap_arg, cargs, pargs)
        return fn(*rargs)

    return _call_closure


def wrap_function(fptr, return_type, args):
    return_type_ctypes = to_ctype(return_type)
    args_ctypes = list(map(to_ctype, args))

    functype = ctypes.CFUNCTYPE(return_type_ctypes, *args_ctypes)

    ctypes.pythonapi.PyCapsule_GetPointer.restype = ctypes.c_void_p
    cfunc = functype(ctypes.pythonapi.PyCapsule_GetPointer(ctypes.py_object(fptr), 0))
    return cfunc


def to_array(arr):
    ctype = type_map_numpy[arr.dtype.char]
    dims = arr.ndim

    if dims == 1:
        data = arr.ctypes.data_as(ctypes.POINTER(ctype))
        return data
    elif dims == 2:
        data = (
            arr.__array_interface__["data"][0]
            + np.arange(arr.shape[0]) * arr.strides[0]
        ).astype(np.uintp)
        return data


def wrap_arg(arg, val):
    return val


def jitbuilder_func(fn):
    typer = asttyper.AstTyper()
    ast = typer(fn)

    argspec = inspect.getfullargspec(fn)
    arg_names = argspec.args
    return_type = (
        argspec.annotations["return"].__name__
        if hasattr(argspec.annotations["return"], "__name__")
        else None
    )
    return specialize(fn.__name__, return_type, arg_names, type_dict, ast)


function_cache = {}


def to_arg_type(arg):
    if isinstance(arg, np.ndarray):
        if arg.ndim > 2:
            raise NotImplementedError(
                "Arrays with dimension > 3 are not currently supported"
            )
        if arg.dtype == np.dtype("int32"):
            return "array_int32" if arg.ndim == 1 else "array2d_int32"
        elif arg.dtype == np.dtype("int64"):
            return "array_int64" if arg.ndim == 1 else "array2d_int64"
        if arg.dtype == np.dtype("float64"):
            return "array_float64" if arg.ndim == 1 else "array2d_float64"
        if arg.dtype == np.dtype("float32"):
            return "array_float32" if arg.ndim == 1 else "array2d_float"
        if arg.dtype == np.dtype("uint64"):
            return "array2d_float64"
    if isinstance(arg, int) and arg <= sys.maxsize:
        return "int64"
    elif isinstance(arg, float):
        return "float"
    elif isinstance(arg, str):
        return "string"
    elif isinstance(arg, ctypes.POINTER(ctypes.c_int64)):
        return "array_int64"
    else:
        raise Exception(f"Type not supported: {type(arg)}")


def specialize(func_name, return_type, arg_names, td, ast):
    def _wrapper(*args):
        key = func_name  # mangle_name(func_name, params)
        if key in function_cache:
            return function_cache[key](*args)
        else:
            types = list(map(to_arg_type, list(args)))
            params = dict(zip(arg_names, types))

            fptr = codegen(return_type, func_name, params, td, ast)
            cfunc = wrap_function(fptr, return_type, types)
            function_cache[key] = cfunc
            return cfunc(*args)

    return _wrapper


def codegen(return_type, func_name, params, td, ast):
    # return_type, func_name, params, td, ast
    method = JitBuilderMethod(return_type, func_name, params, td, ast)
    compiled = jitbuilder.compile_method_builder(method)
    return compiled
