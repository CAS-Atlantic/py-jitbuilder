#include "JitBuilder.hpp"
#include <cstring>
#include <iostream>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class PyIlBuilder : public OMR::JitBuilder::IlBuilder {
public:
  using IlBuilder::IlBuilder;
  bool buildIL() override {
    PYBIND11_OVERLOAD(bool, OMR::JitBuilder::IlBuilder, buildIL, );
  }
};
class PyMethodBuilder : public OMR::JitBuilder::MethodBuilder {
public:
  using MethodBuilder::MethodBuilder;
  bool RequestFunction(const char *name) override {
    PYBIND11_OVERLOAD(bool, OMR::JitBuilder::MethodBuilder, RequestFunction,
                      name);
  }
  bool buildIL() override {
    PYBIND11_OVERLOAD(bool, OMR::JitBuilder::MethodBuilder, buildIL, );
  }
};
class PyVirtualMachineOperandArray
    : public OMR::JitBuilder::VirtualMachineOperandArray {
public:
  using VirtualMachineOperandArray::VirtualMachineOperandArray;
  void Commit(OMR::JitBuilder::IlBuilder *b) override {
    PYBIND11_OVERLOAD(void, OMR::JitBuilder::VirtualMachineOperandArray, Commit,
                      b);
  }
  void Reload(OMR::JitBuilder::IlBuilder *b) override {
    PYBIND11_OVERLOAD(void, OMR::JitBuilder::VirtualMachineOperandArray, Reload,
                      b);
  }
  VirtualMachineState *MakeCopy() override {
    PYBIND11_OVERLOAD(VirtualMachineState *,
                      OMR::JitBuilder::VirtualMachineOperandArray, MakeCopy, );
  }
  void MergeInto(OMR::JitBuilder::VirtualMachineState *vmState,
                 OMR::JitBuilder::IlBuilder *b) override {
    PYBIND11_OVERLOAD(void, OMR::JitBuilder::VirtualMachineOperandArray,
                      MergeInto, vmState, b);
  }
};
class PyVirtualMachineOperandStack
    : public OMR::JitBuilder::VirtualMachineOperandStack {
public:
  using VirtualMachineOperandStack::VirtualMachineOperandStack;
  void Commit(OMR::JitBuilder::IlBuilder *b) override {
    PYBIND11_OVERLOAD(void, OMR::JitBuilder::VirtualMachineOperandStack, Commit,
                      b);
  }
  void Reload(OMR::JitBuilder::IlBuilder *b) override {
    PYBIND11_OVERLOAD(void, OMR::JitBuilder::VirtualMachineOperandStack, Reload,
                      b);
  }
  VirtualMachineState *MakeCopy() override {
    PYBIND11_OVERLOAD(VirtualMachineState *,
                      OMR::JitBuilder::VirtualMachineOperandStack, MakeCopy, );
  }
  void MergeInto(OMR::JitBuilder::VirtualMachineState *vmState,
                 OMR::JitBuilder::IlBuilder *b) override {
    PYBIND11_OVERLOAD(void, OMR::JitBuilder::VirtualMachineOperandStack,
                      MergeInto, vmState, b);
  }
};
class PyVirtualMachineRegister
    : public OMR::JitBuilder::VirtualMachineRegister {
public:
  using VirtualMachineRegister::VirtualMachineRegister;
  void Commit(OMR::JitBuilder::IlBuilder *b) override {
    PYBIND11_OVERLOAD(void, OMR::JitBuilder::VirtualMachineRegister, Commit, b);
  }
  void Reload(OMR::JitBuilder::IlBuilder *b) override {
    PYBIND11_OVERLOAD(void, OMR::JitBuilder::VirtualMachineRegister, Reload, b);
  }
  VirtualMachineState *MakeCopy() override {
    PYBIND11_OVERLOAD(VirtualMachineState *,
                      OMR::JitBuilder::VirtualMachineRegister, MakeCopy, );
  }
};
class PyVirtualMachineRegisterInStruct
    : public OMR::JitBuilder::VirtualMachineRegisterInStruct {
public:
  using VirtualMachineRegisterInStruct::VirtualMachineRegisterInStruct;
  void Commit(OMR::JitBuilder::IlBuilder *b) override {
    PYBIND11_OVERLOAD(void, OMR::JitBuilder::VirtualMachineRegisterInStruct,
                      Commit, b);
  }
  void Reload(OMR::JitBuilder::IlBuilder *b) override {
    PYBIND11_OVERLOAD(void, OMR::JitBuilder::VirtualMachineRegisterInStruct,
                      Reload, b);
  }
  VirtualMachineState *MakeCopy() override {
    PYBIND11_OVERLOAD(VirtualMachineState *,
                      OMR::JitBuilder::VirtualMachineRegisterInStruct,
                      MakeCopy, );
  }
};
class PyVirtualMachineState : public OMR::JitBuilder::VirtualMachineState {
public:
  using VirtualMachineState::VirtualMachineState;
  void Commit(OMR::JitBuilder::IlBuilder *b) override {
    PYBIND11_OVERLOAD(void, OMR::JitBuilder::VirtualMachineState, Commit, b);
  }
  void Reload(OMR::JitBuilder::IlBuilder *b) override {
    PYBIND11_OVERLOAD(void, OMR::JitBuilder::VirtualMachineState, Reload, b);
  }
  VirtualMachineState *MakeCopy() override {
    PYBIND11_OVERLOAD(VirtualMachineState *,
                      OMR::JitBuilder::VirtualMachineState, MakeCopy, );
  }
  void MergeInto(OMR::JitBuilder::VirtualMachineState *vmState,
                 OMR::JitBuilder::IlBuilder *b) override {
    PYBIND11_OVERLOAD(void, OMR::JitBuilder::VirtualMachineState, MergeInto,
                      vmState, b);
  }
};
void *compileMethodBuilder_wrapper(OMR::JitBuilder::MethodBuilder mb) {
  void *compiledMethod = 0;
  auto rc = compileMethodBuilder(&mb, &compiledMethod);

  if (rc != 0) {
    std::cerr << "FAIL: compilation error " << rc << "\n";
    // exit(-2);
  }

  return compiledMethod;
}
PYBIND11_MODULE(jitbuilder, m) {
  m.def("initialize_jit", &initializeJit);
  m.def("initialize_jit_with_options", &initializeJitWithOptions);
  m.def("compile_method_builder", &compileMethodBuilder_wrapper);
  m.def("shutdown_jit", &shutdownJit);

  py::class_<OMR::JitBuilder::IlBuilder, PyIlBuilder>(m, "IlBuilder")
      .def_readwrite("no_type", &OMR::JitBuilder::IlBuilder::NoType)
      .def_readwrite("int_8", &OMR::JitBuilder::IlBuilder::Int8)
      .def_readwrite("int_16", &OMR::JitBuilder::IlBuilder::Int16)
      .def_readwrite("int_32", &OMR::JitBuilder::IlBuilder::Int32)
      .def_readwrite("int_64", &OMR::JitBuilder::IlBuilder::Int64)
      .def_readwrite("float", &OMR::JitBuilder::IlBuilder::Float)
      .def_readwrite("double", &OMR::JitBuilder::IlBuilder::Double)
      .def_readwrite("address", &OMR::JitBuilder::IlBuilder::Address)
      .def_readwrite("vector_int_8", &OMR::JitBuilder::IlBuilder::VectorInt8)
      .def_readwrite("vector_int_16", &OMR::JitBuilder::IlBuilder::VectorInt16)
      .def_readwrite("vector_int_32", &OMR::JitBuilder::IlBuilder::VectorInt32)
      .def_readwrite("vector_int_64", &OMR::JitBuilder::IlBuilder::VectorInt64)
      .def_readwrite("vector_float", &OMR::JitBuilder::IlBuilder::VectorFloat)
      .def_readwrite("vector_double", &OMR::JitBuilder::IlBuilder::VectorDouble)
      .def_readwrite("word", &OMR::JitBuilder::IlBuilder::Word)
      .def_readwrite("impl", &OMR::JitBuilder::IlBuilder::_impl)
      .def(py::init<void *>())
      .def("buildIL", &OMR::JitBuilder::IlBuilder::buildIL)
      .def("orphan_builder", &OMR::JitBuilder::IlBuilder::OrphanBuilder)
      .def("orphan_bytecode_builder",
           [](OMR::JitBuilder::IlBuilder *self, int32_t bcIndex, char *name) {
             char *name_wrapper = new char[strlen(name)];
             strcpy(name_wrapper, name);

             return self->OrphanBytecodeBuilder(bcIndex, name_wrapper);
           })
      .def("copy", &OMR::JitBuilder::IlBuilder::Copy)
      .def("type_dictionary", &OMR::JitBuilder::IlBuilder::typeDictionary)
      .def("const_integer", &OMR::JitBuilder::IlBuilder::ConstInteger)
      .def("const_address", &OMR::JitBuilder::IlBuilder::ConstAddress)
      .def("const_double", &OMR::JitBuilder::IlBuilder::ConstDouble)
      .def("const_float", &OMR::JitBuilder::IlBuilder::ConstFloat)
      .def("const_int_8", &OMR::JitBuilder::IlBuilder::ConstInt8)
      .def("const_int_16", &OMR::JitBuilder::IlBuilder::ConstInt16)
      .def("const_int_32", &OMR::JitBuilder::IlBuilder::ConstInt32,
           py::return_value_policy::reference)
      .def("const_int_64", &OMR::JitBuilder::IlBuilder::ConstInt64)
      .def("const_string",
           [](OMR::JitBuilder::IlBuilder *self, char *value) {
             char *value_wrapper = new char[strlen(value)];
             strcpy(value_wrapper, value);

             return self->ConstString(value_wrapper);
           },
           py::return_value_policy::reference)
      .def("const",
           py::overload_cast<void *>(&OMR::JitBuilder::IlBuilder::Const))
      .def("const",
           py::overload_cast<double>(&OMR::JitBuilder::IlBuilder::Const))
      .def("const",
           py::overload_cast<float>(&OMR::JitBuilder::IlBuilder::Const))
      .def("const",
           py::overload_cast<int8_t>(&OMR::JitBuilder::IlBuilder::Const))
      .def("const",
           py::overload_cast<int16_t>(&OMR::JitBuilder::IlBuilder::Const))
      .def("const",
           py::overload_cast<int32_t>(&OMR::JitBuilder::IlBuilder::Const))
      .def("const",
           py::overload_cast<int64_t>(&OMR::JitBuilder::IlBuilder::Const))
      .def("null_address", &OMR::JitBuilder::IlBuilder::NullAddress)
      .def("add", &OMR::JitBuilder::IlBuilder::Add,
           py::return_value_policy::reference)
      .def("add_with_overflow",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *overflowHandler,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::IlBuilder **overflowHandler_wrapper =
                 &overflowHandler;
             return py::make_tuple(
                 self->AddWithOverflow(overflowHandler_wrapper, left, right),
                 overflowHandler);
           })
      .def("add_with_unsigned_overflow",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *overflowHandler,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::IlBuilder **overflowHandler_wrapper =
                 &overflowHandler;
             return py::make_tuple(self->AddWithUnsignedOverflow(
                                       overflowHandler_wrapper, left, right),
                                   overflowHandler);
           })
      .def("and_", &OMR::JitBuilder::IlBuilder::And,
           py::return_value_policy::reference)
      .def("div", &OMR::JitBuilder::IlBuilder::Div,
           py::return_value_policy::reference)
      .def("index_at", &OMR::JitBuilder::IlBuilder::IndexAt,
           py::return_value_policy::reference)
      .def("mul", &OMR::JitBuilder::IlBuilder::Mul,
           py::return_value_policy::reference)
      .def("mul_with_overflow",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *overflowHandler,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::IlBuilder **overflowHandler_wrapper =
                 &overflowHandler;
             return py::make_tuple(
                 self->MulWithOverflow(overflowHandler_wrapper, left, right),
                 overflowHandler);
           })
      .def("negate", &OMR::JitBuilder::IlBuilder::Negate)
      .def("or_", &OMR::JitBuilder::IlBuilder::Or,
           py::return_value_policy::reference)
      .def("rem", &OMR::JitBuilder::IlBuilder::Rem)
      .def("shift_l", &OMR::JitBuilder::IlBuilder::ShiftL)
      .def("shift_r", &OMR::JitBuilder::IlBuilder::ShiftR)
      .def("sub", &OMR::JitBuilder::IlBuilder::Sub,
           py::return_value_policy::reference)
      .def("sub_with_overflow",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *overflowHandler,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::IlBuilder **overflowHandler_wrapper =
                 &overflowHandler;
             return py::make_tuple(
                 self->SubWithOverflow(overflowHandler_wrapper, left, right),
                 overflowHandler);
           })
      .def("sub_with_unsigned_overflow",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *overflowHandler,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::IlBuilder **overflowHandler_wrapper =
                 &overflowHandler;
             return py::make_tuple(self->SubWithUnsignedOverflow(
                                       overflowHandler_wrapper, left, right),
                                   overflowHandler);
           })
      .def("unsigned_shift_r", &OMR::JitBuilder::IlBuilder::UnsignedShiftR)
      .def("xor", &OMR::JitBuilder::IlBuilder::Xor)
      .def("equal_to", &OMR::JitBuilder::IlBuilder::EqualTo,
           py::return_value_policy::reference)
      .def("less_or_equal_to", &OMR::JitBuilder::IlBuilder::LessOrEqualTo,
           py::return_value_policy::reference)
      .def("less_than", &OMR::JitBuilder::IlBuilder::LessThan,
           py::return_value_policy::reference)
      .def("greater_or_equal_to", &OMR::JitBuilder::IlBuilder::GreaterOrEqualTo,
           py::return_value_policy::reference)
      .def("greater_than", &OMR::JitBuilder::IlBuilder::GreaterThan,
           py::return_value_policy::reference)
      .def("not_equal_to", &OMR::JitBuilder::IlBuilder::NotEqualTo,
           py::return_value_policy::reference)
      .def("unsigned_less_or_equal_to",
           &OMR::JitBuilder::IlBuilder::UnsignedLessOrEqualTo)
      .def("unsigned_less_than", &OMR::JitBuilder::IlBuilder::UnsignedLessThan)
      .def("unsigned_greater_or_equal_to",
           &OMR::JitBuilder::IlBuilder::UnsignedGreaterOrEqualTo)
      .def("unsigned_greater_than",
           &OMR::JitBuilder::IlBuilder::UnsignedGreaterThan)
      .def("convert_bits_to", &OMR::JitBuilder::IlBuilder::ConvertBitsTo)
      .def("convert_to", &OMR::JitBuilder::IlBuilder::ConvertTo)
      .def("unsigned_convert_to",
           &OMR::JitBuilder::IlBuilder::UnsignedConvertTo)
      .def("atomic_add", &OMR::JitBuilder::IlBuilder::AtomicAdd)
      .def("create_local_array", &OMR::JitBuilder::IlBuilder::CreateLocalArray)
      .def("create_local_struct",
           &OMR::JitBuilder::IlBuilder::CreateLocalStruct)
      .def("load",
           [](OMR::JitBuilder::IlBuilder *self, const char *name) {
             char *name_wrapper = new char[strlen(name)];
             strcpy(name_wrapper, name);

             return self->Load(name_wrapper);
           },
           py::return_value_policy::reference)
      .def("load_at", &OMR::JitBuilder::IlBuilder::LoadAt,
           py::return_value_policy::reference)
      .def("load_indirect",
           [](OMR::JitBuilder::IlBuilder *self, const char *type,
              const char *field, OMR::JitBuilder::IlValue *object) {
             char *type_wrapper = new char[strlen(type)];
             strcpy(type_wrapper, type);

             char *field_wrapper = new char[strlen(field)];
             strcpy(field_wrapper, field);

             return self->LoadIndirect(type_wrapper, field_wrapper, object);
           },
           py::return_value_policy::reference)
      .def("store",
           [](OMR::JitBuilder::IlBuilder *self, const char *name,
              OMR::JitBuilder::IlValue *value) {
             char *name_wrapper = new char[strlen(name)];
             strcpy(name_wrapper, name);

             self->Store(name_wrapper, value);
           })
      .def("store_at", &OMR::JitBuilder::IlBuilder::StoreAt)
      .def("store_indirect",
           [](OMR::JitBuilder::IlBuilder *self, const char *type,
              const char *field, OMR::JitBuilder::IlValue *object,
              OMR::JitBuilder::IlValue *value) {
             char *type_wrapper = new char[strlen(type)];
             strcpy(type_wrapper, type);

             char *field_wrapper = new char[strlen(field)];
             strcpy(field_wrapper, field);

             self->StoreIndirect(type_wrapper, field_wrapper, object, value);
           })
      .def("store_over", &OMR::JitBuilder::IlBuilder::StoreOver)
      .def("transaction",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *persistentFailureBuilder,
              OMR::JitBuilder::IlBuilder *transientFailureBuilder,
              OMR::JitBuilder::IlBuilder *transactionBuilder) {
             OMR::JitBuilder::IlBuilder **persistentFailureBuilder_wrapper =
                 &persistentFailureBuilder;
             OMR::JitBuilder::IlBuilder **transientFailureBuilder_wrapper =
                 &transientFailureBuilder;
             OMR::JitBuilder::IlBuilder **transactionBuilder_wrapper =
                 &transactionBuilder;
             self->Transaction(persistentFailureBuilder_wrapper,
                               transientFailureBuilder_wrapper,
                               transactionBuilder_wrapper);
             return py::make_tuple(persistentFailureBuilder,
                                   transientFailureBuilder, transactionBuilder);
           })
      .def("transaction_abort", &OMR::JitBuilder::IlBuilder::TransactionAbort)
      .def("struct_field_instance_address",
           [](OMR::JitBuilder::IlBuilder *self, const char *structName,
              const char *fieldName, OMR::JitBuilder::IlValue *obj) {
             char *structName_wrapper = new char[strlen(structName)];
             strcpy(structName_wrapper, structName);

             char *fieldName_wrapper = new char[strlen(fieldName)];
             strcpy(fieldName_wrapper, fieldName);

             return self->StructFieldInstanceAddress(structName_wrapper,
                                                     fieldName_wrapper, obj);
           })
      .def("union_field_instance_address",
           [](OMR::JitBuilder::IlBuilder *self, const char *unionName,
              const char *fieldName, OMR::JitBuilder::IlValue *obj) {
             char *unionName_wrapper = new char[strlen(unionName)];
             strcpy(unionName_wrapper, unionName);

             char *fieldName_wrapper = new char[strlen(fieldName)];
             strcpy(fieldName_wrapper, fieldName);

             return self->UnionFieldInstanceAddress(unionName_wrapper,
                                                    fieldName_wrapper, obj);
           })
      .def("vector_load",
           [](OMR::JitBuilder::IlBuilder *self, char *name) {
             char *name_wrapper = new char[strlen(name)];
             strcpy(name_wrapper, name);

             return self->VectorLoad(name_wrapper);
           })
      .def("vector_load_at", &OMR::JitBuilder::IlBuilder::VectorLoadAt,
           py::return_value_policy::reference)
      .def("vector_store",
           [](OMR::JitBuilder::IlBuilder *self, char *name,
              OMR::JitBuilder::IlValue *value) {
             char *name_wrapper = new char[strlen(name)];
             strcpy(name_wrapper, name);

             self->VectorStore(name_wrapper, value);
           })
      .def("vector_store_at", &OMR::JitBuilder::IlBuilder::VectorStoreAt)
      .def("append_builder", &OMR::JitBuilder::IlBuilder::AppendBuilder)
      .def("call",
           [](OMR::JitBuilder::IlBuilder *self, const char *name,
              int32_t numArgs,
              std::vector<OMR::JitBuilder::IlValue *> arguments) {
             char *name_wrapper = new char[strlen(name)];
             strcpy(name_wrapper, name);

             return self->Call(name_wrapper, numArgs, arguments.data());
           },
           py::return_value_policy::reference)
      .def("call",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::MethodBuilder *name, int32_t numArgs,
              std::vector<OMR::JitBuilder::IlValue *> arguments) {
             return self->Call(name, numArgs, arguments.data());
           },
           py::return_value_policy::reference)
      .def("computed_call",
           [](OMR::JitBuilder::IlBuilder *self, char *name, int32_t numArgs,
              std::vector<OMR::JitBuilder::IlValue *> arguments) {
             char *name_wrapper = new char[strlen(name)];
             strcpy(name_wrapper, name);

             return self->ComputedCall(name_wrapper, numArgs, arguments.data());
           },
           py::return_value_policy::reference)
      .def("do_while_loop",
           [](OMR::JitBuilder::IlBuilder *self, char *exitCondition,
              OMR::JitBuilder::IlBuilder *body) {
             char *exitCondition_wrapper = new char[strlen(exitCondition)];
             strcpy(exitCondition_wrapper, exitCondition);

             OMR::JitBuilder::IlBuilder **body_wrapper = &body;
             self->DoWhileLoop(exitCondition_wrapper, body_wrapper);
             return body;
           })
      .def("do_while_loop",
           [](OMR::JitBuilder::IlBuilder *self, char *exitCondition,
              OMR::JitBuilder::IlBuilder *body,
              OMR::JitBuilder::IlBuilder *breakBuilder,
              OMR::JitBuilder::IlBuilder *continueBuilder) {
             char *exitCondition_wrapper = new char[strlen(exitCondition)];
             strcpy(exitCondition_wrapper, exitCondition);

             OMR::JitBuilder::IlBuilder **body_wrapper = &body;
             OMR::JitBuilder::IlBuilder **breakBuilder_wrapper = &breakBuilder;
             OMR::JitBuilder::IlBuilder **continueBuilder_wrapper =
                 &continueBuilder;
             self->DoWhileLoop(exitCondition_wrapper, body_wrapper,
                               breakBuilder_wrapper, continueBuilder_wrapper);
             return py::make_tuple(body, breakBuilder, continueBuilder);
           })
      .def("do_while_loop_with_break",
           [](OMR::JitBuilder::IlBuilder *self, char *exitCondition,
              OMR::JitBuilder::IlBuilder *body,
              OMR::JitBuilder::IlBuilder *breakBuilder) {
             char *exitCondition_wrapper = new char[strlen(exitCondition)];
             strcpy(exitCondition_wrapper, exitCondition);

             OMR::JitBuilder::IlBuilder **body_wrapper = &body;
             OMR::JitBuilder::IlBuilder **breakBuilder_wrapper = &breakBuilder;
             self->DoWhileLoopWithBreak(exitCondition_wrapper, body_wrapper,
                                        breakBuilder_wrapper);
             return py::make_tuple(body, breakBuilder);
           })
      .def("do_while_loop_with_continue",
           [](OMR::JitBuilder::IlBuilder *self, char *exitCondition,
              OMR::JitBuilder::IlBuilder *body,
              OMR::JitBuilder::IlBuilder *continueBuilder) {
             char *exitCondition_wrapper = new char[strlen(exitCondition)];
             strcpy(exitCondition_wrapper, exitCondition);

             OMR::JitBuilder::IlBuilder **body_wrapper = &body;
             OMR::JitBuilder::IlBuilder **continueBuilder_wrapper =
                 &continueBuilder;
             self->DoWhileLoopWithContinue(exitCondition_wrapper, body_wrapper,
                                           continueBuilder_wrapper);
             return py::make_tuple(body, continueBuilder);
           })
      .def("goto", py::overload_cast<OMR::JitBuilder::IlBuilder *>(
                       &OMR::JitBuilder::IlBuilder::Goto))
      .def("goto",
           [](OMR::JitBuilder::IlBuilder *self, OMR::JitBuilder::IlBuilder *b) {
             OMR::JitBuilder::IlBuilder **b_wrapper = &b;
             self->Goto(b_wrapper);
             return b;
           })
      .def("make_condition", &OMR::JitBuilder::IlBuilder::MakeCondition,
           py::return_value_policy::reference)
      .def("if_and",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *allTrueBuilder,
              OMR::JitBuilder::IlBuilder *anyFalseBuilder, int32_t numTerms,
              std::vector<OMR::JitBuilder::IlBuilder::JBCondition *> terms) {
             OMR::JitBuilder::IlBuilder **allTrueBuilder_wrapper =
                 &allTrueBuilder;
             OMR::JitBuilder::IlBuilder **anyFalseBuilder_wrapper =
                 &anyFalseBuilder;
             self->IfAnd(allTrueBuilder_wrapper, anyFalseBuilder_wrapper,
                         numTerms, terms.data());
             return py::make_tuple(allTrueBuilder, anyFalseBuilder);
           },
           py::return_value_policy::reference)
      .def("if_cmp_equal", py::overload_cast<OMR::JitBuilder::IlBuilder *,
                                             OMR::JitBuilder::IlValue *,
                                             OMR::JitBuilder::IlValue *>(
                               &OMR::JitBuilder::IlBuilder::IfCmpEqual))
      .def("if_cmp_equal",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::IlBuilder **target_wrapper = &target;
             self->IfCmpEqual(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_less_or_equal",
           py::overload_cast<OMR::JitBuilder::IlBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::IlBuilder::IfCmpLessOrEqual))
      .def("if_cmp_less_or_equal",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::IlBuilder **target_wrapper = &target;
             self->IfCmpLessOrEqual(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_less_than", py::overload_cast<OMR::JitBuilder::IlBuilder *,
                                                 OMR::JitBuilder::IlValue *,
                                                 OMR::JitBuilder::IlValue *>(
                                   &OMR::JitBuilder::IlBuilder::IfCmpLessThan))
      .def("if_cmp_less_than",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::IlBuilder **target_wrapper = &target;
             self->IfCmpLessThan(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_greater_or_equal",
           py::overload_cast<OMR::JitBuilder::IlBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::IlBuilder::IfCmpGreaterOrEqual))
      .def("if_cmp_greater_or_equal",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::IlBuilder **target_wrapper = &target;
             self->IfCmpGreaterOrEqual(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_greater_than",
           py::overload_cast<OMR::JitBuilder::IlBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::IlBuilder::IfCmpGreaterThan))
      .def("if_cmp_greater_than",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::IlBuilder **target_wrapper = &target;
             self->IfCmpGreaterThan(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_not_equal", py::overload_cast<OMR::JitBuilder::IlBuilder *,
                                                 OMR::JitBuilder::IlValue *,
                                                 OMR::JitBuilder::IlValue *>(
                                   &OMR::JitBuilder::IlBuilder::IfCmpNotEqual))
      .def("if_cmp_not_equal",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::IlBuilder **target_wrapper = &target;
             self->IfCmpNotEqual(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_unsigned_less_or_equal",
           py::overload_cast<OMR::JitBuilder::IlBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::IlBuilder::IfCmpUnsignedLessOrEqual))
      .def("if_cmp_unsigned_less_or_equal",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::IlBuilder **target_wrapper = &target;
             self->IfCmpUnsignedLessOrEqual(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_unsigned_less_than",
           py::overload_cast<OMR::JitBuilder::IlBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::IlBuilder::IfCmpUnsignedLessThan))
      .def("if_cmp_unsigned_less_than",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::IlBuilder **target_wrapper = &target;
             self->IfCmpUnsignedLessThan(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_unsigned_greater_or_equal",
           py::overload_cast<OMR::JitBuilder::IlBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::IlBuilder::IfCmpUnsignedGreaterOrEqual))
      .def("if_cmp_unsigned_greater_or_equal",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::IlBuilder **target_wrapper = &target;
             self->IfCmpUnsignedGreaterOrEqual(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_unsigned_greater_than",
           py::overload_cast<OMR::JitBuilder::IlBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::IlBuilder::IfCmpUnsignedGreaterThan))
      .def("if_cmp_unsigned_greater_than",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::IlBuilder **target_wrapper = &target;
             self->IfCmpUnsignedGreaterThan(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_equal_zero",
           py::overload_cast<OMR::JitBuilder::IlBuilder *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::IlBuilder::IfCmpEqualZero))
      .def("if_cmp_equal_zero",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *target,
              OMR::JitBuilder::IlValue *condition) {
             OMR::JitBuilder::IlBuilder **target_wrapper = &target;
             self->IfCmpEqualZero(target_wrapper, condition);
             return target;
           })
      .def("if_cmp_not_equal_zero",
           py::overload_cast<OMR::JitBuilder::IlBuilder *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::IlBuilder::IfCmpNotEqualZero))
      .def("if_cmp_not_equal_zero",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *target,
              OMR::JitBuilder::IlValue *condition) {
             OMR::JitBuilder::IlBuilder **target_wrapper = &target;
             self->IfCmpNotEqualZero(target_wrapper, condition);
             return target;
           })
      .def("if_or",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *anyTrueBuilder,
              OMR::JitBuilder::IlBuilder *allFalseBuilder, int32_t numTerms,
              std::vector<OMR::JitBuilder::IlBuilder::JBCondition *> terms) {
             OMR::JitBuilder::IlBuilder **anyTrueBuilder_wrapper =
                 &anyTrueBuilder;
             OMR::JitBuilder::IlBuilder **allFalseBuilder_wrapper =
                 &allFalseBuilder;
             self->IfOr(anyTrueBuilder_wrapper, allFalseBuilder_wrapper,
                        numTerms, terms.data());
             return py::make_tuple(anyTrueBuilder, allFalseBuilder);
           },
           py::return_value_policy::reference)
      .def("if_then",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *thenPath,
              OMR::JitBuilder::IlValue *condition) {
             OMR::JitBuilder::IlBuilder **thenPath_wrapper = &thenPath;
             self->IfThen(thenPath_wrapper, condition);
             return thenPath;
           })
      .def("if_then_else",
           [](OMR::JitBuilder::IlBuilder *self,
              OMR::JitBuilder::IlBuilder *thenPath,
              OMR::JitBuilder::IlBuilder *elsePath,
              OMR::JitBuilder::IlValue *condition) {
             OMR::JitBuilder::IlBuilder **thenPath_wrapper = &thenPath;
             OMR::JitBuilder::IlBuilder **elsePath_wrapper = &elsePath;
             self->IfThenElse(thenPath_wrapper, elsePath_wrapper, condition);
             return py::make_tuple(thenPath, elsePath);
           },
           py::return_value_policy::reference)
      .def("for_loop",
           [](OMR::JitBuilder::IlBuilder *self, bool countsUp, char *indVar,
              OMR::JitBuilder::IlBuilder *body,
              OMR::JitBuilder::IlBuilder *breakBuilder,
              OMR::JitBuilder::IlBuilder *continueBuilder,
              OMR::JitBuilder::IlValue *initial,
              OMR::JitBuilder::IlValue *iterateWhile,
              OMR::JitBuilder::IlValue *increment) {
             char *indVar_wrapper = new char[strlen(indVar)];
             strcpy(indVar_wrapper, indVar);

             OMR::JitBuilder::IlBuilder **body_wrapper = &body;
             OMR::JitBuilder::IlBuilder **breakBuilder_wrapper = &breakBuilder;
             OMR::JitBuilder::IlBuilder **continueBuilder_wrapper =
                 &continueBuilder;
             self->ForLoop(countsUp, indVar_wrapper, body_wrapper,
                           breakBuilder_wrapper, continueBuilder_wrapper,
                           initial, iterateWhile, increment);
             return py::make_tuple(body, breakBuilder, continueBuilder);
           },
           py::return_value_policy::reference)
      .def("for_loop_down",
           [](OMR::JitBuilder::IlBuilder *self, char *indVar,
              OMR::JitBuilder::IlBuilder *body,
              OMR::JitBuilder::IlValue *initial,
              OMR::JitBuilder::IlValue *iterateWhile,
              OMR::JitBuilder::IlValue *increment) {
             char *indVar_wrapper = new char[strlen(indVar)];
             strcpy(indVar_wrapper, indVar);

             OMR::JitBuilder::IlBuilder **body_wrapper = &body;
             self->ForLoopDown(indVar_wrapper, body_wrapper, initial,
                               iterateWhile, increment);
             return body;
           },
           py::return_value_policy::reference)
      .def("for_loop_up",
           [](OMR::JitBuilder::IlBuilder *self, char *indVar,
              OMR::JitBuilder::IlBuilder *body,
              OMR::JitBuilder::IlValue *initial,
              OMR::JitBuilder::IlValue *iterateWhile,
              OMR::JitBuilder::IlValue *increment) {
             char *indVar_wrapper = new char[strlen(indVar)];
             strcpy(indVar_wrapper, indVar);

             OMR::JitBuilder::IlBuilder **body_wrapper = &body;
             self->ForLoopUp(indVar_wrapper, body_wrapper, initial,
                             iterateWhile, increment);
             return body;
           },
           py::return_value_policy::reference)
      .def("for_loop_with_break",
           [](OMR::JitBuilder::IlBuilder *self, bool countsUp, char *indVar,
              OMR::JitBuilder::IlBuilder *body,
              OMR::JitBuilder::IlBuilder *breakBuilder,
              OMR::JitBuilder::IlValue *initial,
              OMR::JitBuilder::IlValue *iterateWhile,
              OMR::JitBuilder::IlValue *increment) {
             char *indVar_wrapper = new char[strlen(indVar)];
             strcpy(indVar_wrapper, indVar);

             OMR::JitBuilder::IlBuilder **body_wrapper = &body;
             OMR::JitBuilder::IlBuilder **breakBuilder_wrapper = &breakBuilder;
             self->ForLoopWithBreak(countsUp, indVar_wrapper, body_wrapper,
                                    breakBuilder_wrapper, initial, iterateWhile,
                                    increment);
             return py::make_tuple(body, breakBuilder);
           },
           py::return_value_policy::reference)
      .def("for_loop_with_continue",
           [](OMR::JitBuilder::IlBuilder *self, bool countsUp, char *indVar,
              OMR::JitBuilder::IlBuilder *body,
              OMR::JitBuilder::IlBuilder *continueBuilder,
              OMR::JitBuilder::IlValue *initial,
              OMR::JitBuilder::IlValue *iterateWhile,
              OMR::JitBuilder::IlValue *increment) {
             char *indVar_wrapper = new char[strlen(indVar)];
             strcpy(indVar_wrapper, indVar);

             OMR::JitBuilder::IlBuilder **body_wrapper = &body;
             OMR::JitBuilder::IlBuilder **continueBuilder_wrapper =
                 &continueBuilder;
             self->ForLoopWithContinue(countsUp, indVar_wrapper, body_wrapper,
                                       continueBuilder_wrapper, initial,
                                       iterateWhile, increment);
             return py::make_tuple(body, continueBuilder);
           },
           py::return_value_policy::reference)
      .def("return_", py::overload_cast<>(&OMR::JitBuilder::IlBuilder::Return),
           py::return_value_policy::reference)
      .def("return_",
           py::overload_cast<OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::IlBuilder::Return),
           py::return_value_policy::reference)
      .def("switch",
           [](OMR::JitBuilder::IlBuilder *self, const char *selectionVar,
              OMR::JitBuilder::IlBuilder *defaultBuilder, int32_t numCases,
              std::vector<OMR::JitBuilder::IlBuilder::JBCase *> cases) {
             char *selectionVar_wrapper = new char[strlen(selectionVar)];
             strcpy(selectionVar_wrapper, selectionVar);

             OMR::JitBuilder::IlBuilder **defaultBuilder_wrapper =
                 &defaultBuilder;
             self->Switch(selectionVar_wrapper, defaultBuilder_wrapper,
                          numCases, cases.data());
             return defaultBuilder;
           })
      .def("make_case",
           [](OMR::JitBuilder::IlBuilder *self, int32_t value,
              OMR::JitBuilder::IlBuilder *builder, int32_t fallsThrough) {
             OMR::JitBuilder::IlBuilder **builder_wrapper = &builder;
             return py::make_tuple(
                 self->MakeCase(value, builder_wrapper, fallsThrough), builder);
           })
      .def("while_do_loop",
           [](OMR::JitBuilder::IlBuilder *self, char *exitCondition,
              OMR::JitBuilder::IlBuilder *body) {
             char *exitCondition_wrapper = new char[strlen(exitCondition)];
             strcpy(exitCondition_wrapper, exitCondition);

             OMR::JitBuilder::IlBuilder **body_wrapper = &body;
             self->WhileDoLoop(exitCondition_wrapper, body_wrapper);
             return body;
           })
      .def("while_do_loop_with_break",
           [](OMR::JitBuilder::IlBuilder *self, char *exitCondition,
              OMR::JitBuilder::IlBuilder *body,
              OMR::JitBuilder::IlBuilder *breakBuilder) {
             char *exitCondition_wrapper = new char[strlen(exitCondition)];
             strcpy(exitCondition_wrapper, exitCondition);

             OMR::JitBuilder::IlBuilder **body_wrapper = &body;
             OMR::JitBuilder::IlBuilder **breakBuilder_wrapper = &breakBuilder;
             self->WhileDoLoopWithBreak(exitCondition_wrapper, body_wrapper,
                                        breakBuilder_wrapper);
             return py::make_tuple(body, breakBuilder);
           })
      .def("while_do_loop_with_continue",
           [](OMR::JitBuilder::IlBuilder *self, char *exitCondition,
              OMR::JitBuilder::IlBuilder *body,
              OMR::JitBuilder::IlBuilder *continueBuilder) {
             char *exitCondition_wrapper = new char[strlen(exitCondition)];
             strcpy(exitCondition_wrapper, exitCondition);

             OMR::JitBuilder::IlBuilder **body_wrapper = &body;
             OMR::JitBuilder::IlBuilder **continueBuilder_wrapper =
                 &continueBuilder;
             self->WhileDoLoopWithContinue(exitCondition_wrapper, body_wrapper,
                                           continueBuilder_wrapper);
             return py::make_tuple(body, continueBuilder);
           })
      .def("allocate_il_builder", &OMR::JitBuilder::allocateIlBuilder);

  py::class_<OMR::JitBuilder::IlBuilder::JBCase>(m, "JBCase")
      .def_readwrite("impl", &OMR::JitBuilder::IlBuilder::JBCase::_impl)
      .def(py::init<int32_t, OMR::JitBuilder::IlBuilder *, int32_t>())
      .def(py::init<void *>())
      .def("allocate_il_builder_jbcase",
           &OMR::JitBuilder::allocateIlBuilderJBCase);

  py::class_<OMR::JitBuilder::IlBuilder::JBCondition>(m, "JBCondition")
      .def_readwrite("impl", &OMR::JitBuilder::IlBuilder::JBCondition::_impl)
      .def(py::init<OMR::JitBuilder::IlBuilder *, OMR::JitBuilder::IlValue *>())
      .def(py::init<void *>())
      .def("allocate_il_builder_jbcondition",
           &OMR::JitBuilder::allocateIlBuilderJBCondition);

  py::class_<OMR::JitBuilder::IlType>(m, "IlType")
      .def_readwrite("impl", &OMR::JitBuilder::IlType::_impl)
      .def(py::init<void *>())
      .def("base_type", &OMR::JitBuilder::IlType::baseType)
      .def("get_name", &OMR::JitBuilder::IlType::getName)
      .def("primitive_type", &OMR::JitBuilder::IlType::primitiveType)
      .def("get_size", &OMR::JitBuilder::IlType::getSize)
      .def("is_pointer", &OMR::JitBuilder::IlType::isPointer)
      .def("allocate_il_type", &OMR::JitBuilder::allocateIlType);

  py::class_<OMR::JitBuilder::IlValue>(m, "IlValue")
      .def_readwrite("impl", &OMR::JitBuilder::IlValue::_impl)
      .def(py::init<void *>())
      .def("get_id", &OMR::JitBuilder::IlValue::getID)
      .def("allocate_il_value", &OMR::JitBuilder::allocateIlValue);

  py::class_<OMR::JitBuilder::TypeDictionary>(m, "TypeDictionary")
      .def_readwrite("no_type", &OMR::JitBuilder::TypeDictionary::NoType)
      .def_readwrite("int_8", &OMR::JitBuilder::TypeDictionary::Int8)
      .def_readwrite("int_16", &OMR::JitBuilder::TypeDictionary::Int16)
      .def_readwrite("int_32", &OMR::JitBuilder::TypeDictionary::Int32)
      .def_readwrite("int_64", &OMR::JitBuilder::TypeDictionary::Int64)
      .def_readwrite("float", &OMR::JitBuilder::TypeDictionary::Float)
      .def_readwrite("double", &OMR::JitBuilder::TypeDictionary::Double)
      .def_readwrite("address", &OMR::JitBuilder::TypeDictionary::Address)
      .def_readwrite("vector_int_8",
                     &OMR::JitBuilder::TypeDictionary::VectorInt8)
      .def_readwrite("vector_int_16",
                     &OMR::JitBuilder::TypeDictionary::VectorInt16)
      .def_readwrite("vector_int_32",
                     &OMR::JitBuilder::TypeDictionary::VectorInt32)
      .def_readwrite("vector_int_64",
                     &OMR::JitBuilder::TypeDictionary::VectorInt64)
      .def_readwrite("vector_float",
                     &OMR::JitBuilder::TypeDictionary::VectorFloat)
      .def_readwrite("vector_double",
                     &OMR::JitBuilder::TypeDictionary::VectorDouble)
      .def_readwrite("word", &OMR::JitBuilder::TypeDictionary::Word)
      .def_readwrite("p_no_type", &OMR::JitBuilder::TypeDictionary::pNoType)
      .def_readwrite("p_int_8", &OMR::JitBuilder::TypeDictionary::pInt8)
      .def_readwrite("p_int_16", &OMR::JitBuilder::TypeDictionary::pInt16)
      .def_readwrite("p_int_32", &OMR::JitBuilder::TypeDictionary::pInt32)
      .def_readwrite("p_int_64", &OMR::JitBuilder::TypeDictionary::pInt64)
      .def_readwrite("p_float", &OMR::JitBuilder::TypeDictionary::pFloat)
      .def_readwrite("p_double", &OMR::JitBuilder::TypeDictionary::pDouble)
      .def_readwrite("p_address", &OMR::JitBuilder::TypeDictionary::pAddress)
      .def_readwrite("p_vector_int_8",
                     &OMR::JitBuilder::TypeDictionary::pVectorInt8)
      .def_readwrite("p_vector_int_16",
                     &OMR::JitBuilder::TypeDictionary::pVectorInt16)
      .def_readwrite("p_vector_int_32",
                     &OMR::JitBuilder::TypeDictionary::pVectorInt32)
      .def_readwrite("p_vector_int_64",
                     &OMR::JitBuilder::TypeDictionary::pVectorInt64)
      .def_readwrite("p_vector_float",
                     &OMR::JitBuilder::TypeDictionary::pVectorFloat)
      .def_readwrite("p_vector_double",
                     &OMR::JitBuilder::TypeDictionary::pVectorDouble)
      .def_readwrite("p_word", &OMR::JitBuilder::TypeDictionary::pWord)
      .def_readwrite("impl", &OMR::JitBuilder::TypeDictionary::_impl)
      .def(py::init<>())
      .def(py::init<void *>())
      .def("close_struct",
           [](OMR::JitBuilder::TypeDictionary *self, const char *structName) {
             char *structName_wrapper = new char[strlen(structName)];
             strcpy(structName_wrapper, structName);

             self->CloseStruct(structName_wrapper);
           })
      .def("close_struct",
           [](OMR::JitBuilder::TypeDictionary *self, const char *structName,
              size_t size) {
             char *structName_wrapper = new char[strlen(structName)];
             strcpy(structName_wrapper, structName);

             self->CloseStruct(structName_wrapper, size);
           })
      .def("close_union",
           [](OMR::JitBuilder::TypeDictionary *self, const char *unionName) {
             char *unionName_wrapper = new char[strlen(unionName)];
             strcpy(unionName_wrapper, unionName);

             self->CloseUnion(unionName_wrapper);
           })
      .def("define_field",
           [](OMR::JitBuilder::TypeDictionary *self, const char *structName,
              const char *fieldName, OMR::JitBuilder::IlType *type) {
             char *structName_wrapper = new char[strlen(structName)];
             strcpy(structName_wrapper, structName);

             char *fieldName_wrapper = new char[strlen(fieldName)];
             strcpy(fieldName_wrapper, fieldName);

             self->DefineField(structName_wrapper, fieldName_wrapper, type);
           })
      .def("define_field",
           [](OMR::JitBuilder::TypeDictionary *self, const char *structName,
              const char *fieldName, OMR::JitBuilder::IlType *type,
              size_t offset) {
             char *structName_wrapper = new char[strlen(structName)];
             strcpy(structName_wrapper, structName);

             char *fieldName_wrapper = new char[strlen(fieldName)];
             strcpy(fieldName_wrapper, fieldName);

             self->DefineField(structName_wrapper, fieldName_wrapper, type,
                               offset);
           })
      .def("define_struct",
           [](OMR::JitBuilder::TypeDictionary *self, const char *structName) {
             char *structName_wrapper = new char[strlen(structName)];
             strcpy(structName_wrapper, structName);

             return self->DefineStruct(structName_wrapper);
           },
           py::return_value_policy::reference)
      .def("define_union",
           [](OMR::JitBuilder::TypeDictionary *self, const char *unionName) {
             char *unionName_wrapper = new char[strlen(unionName)];
             strcpy(unionName_wrapper, unionName);

             return self->DefineUnion(unionName_wrapper);
           })
      .def("get_field_type",
           [](OMR::JitBuilder::TypeDictionary *self, const char *structName,
              const char *fieldName) {
             char *structName_wrapper = new char[strlen(structName)];
             strcpy(structName_wrapper, structName);

             char *fieldName_wrapper = new char[strlen(fieldName)];
             strcpy(fieldName_wrapper, fieldName);

             return self->GetFieldType(structName_wrapper, fieldName_wrapper);
           })
      .def("lookup_struct",
           [](OMR::JitBuilder::TypeDictionary *self, const char *structName) {
             char *structName_wrapper = new char[strlen(structName)];
             strcpy(structName_wrapper, structName);

             return self->LookupStruct(structName_wrapper);
           },
           py::return_value_policy::reference)
      .def("lookup_union",
           [](OMR::JitBuilder::TypeDictionary *self, const char *unionName) {
             char *unionName_wrapper = new char[strlen(unionName)];
             strcpy(unionName_wrapper, unionName);

             return self->LookupUnion(unionName_wrapper);
           })
      .def("offset_of",
           [](OMR::JitBuilder::TypeDictionary *self, const char *structName,
              const char *fieldName) {
             char *structName_wrapper = new char[strlen(structName)];
             strcpy(structName_wrapper, structName);

             char *fieldName_wrapper = new char[strlen(fieldName)];
             strcpy(fieldName_wrapper, fieldName);

             return self->OffsetOf(structName_wrapper, fieldName_wrapper);
           })
      .def("pointer_to", py::overload_cast<OMR::JitBuilder::IlType *>(
                             &OMR::JitBuilder::TypeDictionary::PointerTo))
      .def("pointer_to",
           [](OMR::JitBuilder::TypeDictionary *self, const char *structName) {
             char *structName_wrapper = new char[strlen(structName)];
             strcpy(structName_wrapper, structName);

             return self->PointerTo(structName_wrapper);
           })
      .def("union_field",
           [](OMR::JitBuilder::TypeDictionary *self, const char *unionName,
              const char *fieldName, OMR::JitBuilder::IlType *fieldType) {
             char *unionName_wrapper = new char[strlen(unionName)];
             strcpy(unionName_wrapper, unionName);

             char *fieldName_wrapper = new char[strlen(fieldName)];
             strcpy(fieldName_wrapper, fieldName);

             self->UnionField(unionName_wrapper, fieldName_wrapper, fieldType);
           })
      .def("union_field_type",
           [](OMR::JitBuilder::TypeDictionary *self, const char *unionName,
              const char *fieldName) {
             char *unionName_wrapper = new char[strlen(unionName)];
             strcpy(unionName_wrapper, unionName);

             char *fieldName_wrapper = new char[strlen(fieldName)];
             strcpy(fieldName_wrapper, fieldName);

             return self->UnionFieldType(unionName_wrapper, fieldName_wrapper);
           })
      .def("allocate_type_dictionary",
           &OMR::JitBuilder::allocateTypeDictionary);

  py::class_<OMR::JitBuilder::VirtualMachineState, PyVirtualMachineState>(
      m, "VirtualMachineState")
      .def_readwrite("impl", &OMR::JitBuilder::VirtualMachineState::_impl)
      .def(py::init<>())
      .def(py::init<void *>())
      .def("commit", &OMR::JitBuilder::VirtualMachineState::Commit)
      .def("reload", &OMR::JitBuilder::VirtualMachineState::Reload)
      .def("make_copy", &OMR::JitBuilder::VirtualMachineState::MakeCopy)
      .def("merge_into", &OMR::JitBuilder::VirtualMachineState::MergeInto)
      .def("allocate_virtual_machine_state",
           &OMR::JitBuilder::allocateVirtualMachineState);

  py::class_<OMR::JitBuilder::BytecodeBuilder, OMR::JitBuilder::IlBuilder>(
      m, "BytecodeBuilder")
      .def(py::init<void *>())
      .def("bc_index", &OMR::JitBuilder::BytecodeBuilder::bcIndex)
      .def("name", &OMR::JitBuilder::BytecodeBuilder::name)
      .def("vm_state", &OMR::JitBuilder::BytecodeBuilder::vmState)
      .def("add_fall_through_builder",
           &OMR::JitBuilder::BytecodeBuilder::AddFallThroughBuilder)
      .def("add_successor_builders",
           [](OMR::JitBuilder::BytecodeBuilder *self, uint32_t numBuilders,
              std::vector<OMR::JitBuilder::BytecodeBuilder *> builders) {
             self->AddSuccessorBuilders(numBuilders, builders.data());
           })
      .def("add_successor_builder",
           [](OMR::JitBuilder::BytecodeBuilder *self,
              OMR::JitBuilder::BytecodeBuilder *b) {
             OMR::JitBuilder::BytecodeBuilder **b_wrapper = &b;
             self->AddSuccessorBuilder(b_wrapper);
             return b;
           })
      .def("goto", py::overload_cast<OMR::JitBuilder::BytecodeBuilder *>(
                       &OMR::JitBuilder::BytecodeBuilder::Goto))
      .def("goto",
           [](OMR::JitBuilder::BytecodeBuilder *self,
              OMR::JitBuilder::BytecodeBuilder *b) {
             OMR::JitBuilder::BytecodeBuilder **b_wrapper = &b;
             self->Goto(b_wrapper);
             return b;
           })
      .def("if_cmp_equal", py::overload_cast<OMR::JitBuilder::BytecodeBuilder *,
                                             OMR::JitBuilder::IlValue *,
                                             OMR::JitBuilder::IlValue *>(
                               &OMR::JitBuilder::BytecodeBuilder::IfCmpEqual))
      .def("if_cmp_equal",
           [](OMR::JitBuilder::BytecodeBuilder *self,
              OMR::JitBuilder::BytecodeBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::BytecodeBuilder **target_wrapper = &target;
             self->IfCmpEqual(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_less_or_equal",
           py::overload_cast<OMR::JitBuilder::BytecodeBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::BytecodeBuilder::IfCmpLessOrEqual))
      .def("if_cmp_less_or_equal",
           [](OMR::JitBuilder::BytecodeBuilder *self,
              OMR::JitBuilder::BytecodeBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::BytecodeBuilder **target_wrapper = &target;
             self->IfCmpLessOrEqual(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_less_than",
           py::overload_cast<OMR::JitBuilder::BytecodeBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::BytecodeBuilder::IfCmpLessThan))
      .def("if_cmp_less_than",
           [](OMR::JitBuilder::BytecodeBuilder *self,
              OMR::JitBuilder::BytecodeBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::BytecodeBuilder **target_wrapper = &target;
             self->IfCmpLessThan(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_greater_or_equal",
           py::overload_cast<OMR::JitBuilder::BytecodeBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::BytecodeBuilder::IfCmpGreaterOrEqual))
      .def("if_cmp_greater_or_equal",
           [](OMR::JitBuilder::BytecodeBuilder *self,
              OMR::JitBuilder::BytecodeBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::BytecodeBuilder **target_wrapper = &target;
             self->IfCmpGreaterOrEqual(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_greater_than",
           py::overload_cast<OMR::JitBuilder::BytecodeBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::BytecodeBuilder::IfCmpGreaterThan))
      .def("if_cmp_greater_than",
           [](OMR::JitBuilder::BytecodeBuilder *self,
              OMR::JitBuilder::BytecodeBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::BytecodeBuilder **target_wrapper = &target;
             self->IfCmpGreaterThan(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_not_equal",
           py::overload_cast<OMR::JitBuilder::BytecodeBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::BytecodeBuilder::IfCmpNotEqual))
      .def("if_cmp_not_equal",
           [](OMR::JitBuilder::BytecodeBuilder *self,
              OMR::JitBuilder::BytecodeBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::BytecodeBuilder **target_wrapper = &target;
             self->IfCmpNotEqual(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_unsigned_less_or_equal",
           py::overload_cast<OMR::JitBuilder::BytecodeBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::BytecodeBuilder::IfCmpUnsignedLessOrEqual))
      .def("if_cmp_unsigned_less_or_equal",
           [](OMR::JitBuilder::BytecodeBuilder *self,
              OMR::JitBuilder::BytecodeBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::BytecodeBuilder **target_wrapper = &target;
             self->IfCmpUnsignedLessOrEqual(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_unsigned_less_than",
           py::overload_cast<OMR::JitBuilder::BytecodeBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::BytecodeBuilder::IfCmpUnsignedLessThan))
      .def("if_cmp_unsigned_less_than",
           [](OMR::JitBuilder::BytecodeBuilder *self,
              OMR::JitBuilder::BytecodeBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::BytecodeBuilder **target_wrapper = &target;
             self->IfCmpUnsignedLessThan(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_unsigned_greater_or_equal",
           py::overload_cast<OMR::JitBuilder::BytecodeBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::BytecodeBuilder::IfCmpUnsignedGreaterOrEqual))
      .def("if_cmp_unsigned_greater_or_equal",
           [](OMR::JitBuilder::BytecodeBuilder *self,
              OMR::JitBuilder::BytecodeBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::BytecodeBuilder **target_wrapper = &target;
             self->IfCmpUnsignedGreaterOrEqual(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_unsigned_greater_than",
           py::overload_cast<OMR::JitBuilder::BytecodeBuilder *,
                             OMR::JitBuilder::IlValue *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::BytecodeBuilder::IfCmpUnsignedGreaterThan))
      .def("if_cmp_unsigned_greater_than",
           [](OMR::JitBuilder::BytecodeBuilder *self,
              OMR::JitBuilder::BytecodeBuilder *target,
              OMR::JitBuilder::IlValue *left, OMR::JitBuilder::IlValue *right) {
             OMR::JitBuilder::BytecodeBuilder **target_wrapper = &target;
             self->IfCmpUnsignedGreaterThan(target_wrapper, left, right);
             return target;
           })
      .def("if_cmp_equal_zero",
           py::overload_cast<OMR::JitBuilder::BytecodeBuilder *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::BytecodeBuilder::IfCmpEqualZero))
      .def("if_cmp_equal_zero",
           [](OMR::JitBuilder::BytecodeBuilder *self,
              OMR::JitBuilder::BytecodeBuilder *target,
              OMR::JitBuilder::IlValue *condition) {
             OMR::JitBuilder::BytecodeBuilder **target_wrapper = &target;
             self->IfCmpEqualZero(target_wrapper, condition);
             return target;
           })
      .def("if_cmp_not_equal_zero",
           py::overload_cast<OMR::JitBuilder::BytecodeBuilder *,
                             OMR::JitBuilder::IlValue *>(
               &OMR::JitBuilder::BytecodeBuilder::IfCmpNotEqualZero))
      .def("if_cmp_not_equal_zero",
           [](OMR::JitBuilder::BytecodeBuilder *self,
              OMR::JitBuilder::BytecodeBuilder *target,
              OMR::JitBuilder::IlValue *condition) {
             OMR::JitBuilder::BytecodeBuilder **target_wrapper = &target;
             self->IfCmpNotEqualZero(target_wrapper, condition);
             return target;
           })
      .def("allocate_bytecode_builder",
           &OMR::JitBuilder::allocateBytecodeBuilder);

  py::class_<OMR::JitBuilder::MethodBuilder, PyMethodBuilder,
             OMR::JitBuilder::IlBuilder>(m, "MethodBuilder")
      .def(py::init<OMR::JitBuilder::TypeDictionary *>())
      .def(py::init<OMR::JitBuilder::TypeDictionary *,
                    OMR::JitBuilder::VirtualMachineState *>())
      .def(py::init<OMR::JitBuilder::MethodBuilder *>())
      .def(py::init<OMR::JitBuilder::MethodBuilder *,
                    OMR::JitBuilder::VirtualMachineState *>())
      .def(py::init<void *>())
      .def("request_function",
           [](OMR::JitBuilder::MethodBuilder *self, char *name) {
             char *name_wrapper = new char[strlen(name)];
             strcpy(name_wrapper, name);

             return self->RequestFunction(name_wrapper);
           })
      .def("all_locals_have_been_defined",
           &OMR::JitBuilder::MethodBuilder::AllLocalsHaveBeenDefined)
      .def("append_builder",
           py::overload_cast<OMR::JitBuilder::IlBuilder *>(
               &OMR::JitBuilder::MethodBuilder::AppendBuilder))
      .def("append_builder",
           py::overload_cast<OMR::JitBuilder::BytecodeBuilder *>(
               &OMR::JitBuilder::MethodBuilder::AppendBuilder))
      .def("append_bytecode_builder",
           &OMR::JitBuilder::MethodBuilder::AppendBytecodeBuilder)
      .def("define_file",
           [](OMR::JitBuilder::MethodBuilder *self, const char *fileName) {
             char *fileName_wrapper = new char[strlen(fileName)];
             strcpy(fileName_wrapper, fileName);

             self->DefineFile(fileName_wrapper);
           })
      .def("define_line",
           [](OMR::JitBuilder::MethodBuilder *self, const char *line) {
             char *line_wrapper = new char[strlen(line)];
             strcpy(line_wrapper, line);

             self->DefineLine(line_wrapper);
           })
      .def("define_line", py::overload_cast<int32_t>(
                              &OMR::JitBuilder::MethodBuilder::DefineLine))
      .def("define_name",
           [](OMR::JitBuilder::MethodBuilder *self, const char *name) {
             char *name_wrapper = new char[strlen(name)];
             strcpy(name_wrapper, name);

             self->DefineName(name_wrapper);
           })
      .def("define_parameter",
           [](OMR::JitBuilder::MethodBuilder *self, const char *name,
              OMR::JitBuilder::IlType *type) {
             char *name_wrapper = new char[strlen(name)];
             strcpy(name_wrapper, name);

             self->DefineParameter(name_wrapper, type);
           })
      .def("define_array_parameter",
           [](OMR::JitBuilder::MethodBuilder *self, const char *name,
              OMR::JitBuilder::IlType *type) {
             char *name_wrapper = new char[strlen(name)];
             strcpy(name_wrapper, name);

             self->DefineArrayParameter(name_wrapper, type);
           })
      .def("define_return_type",
           &OMR::JitBuilder::MethodBuilder::DefineReturnType)
      .def("define_local",
           [](OMR::JitBuilder::MethodBuilder *self, const char *name,
              OMR::JitBuilder::IlType *type) {
             char *name_wrapper = new char[strlen(name)];
             strcpy(name_wrapper, name);

             self->DefineLocal(name_wrapper, type);
           })
      .def("define_memory",
           [](OMR::JitBuilder::MethodBuilder *self, const char *name,
              OMR::JitBuilder::IlType *type, void *location) {
             char *name_wrapper = new char[strlen(name)];
             strcpy(name_wrapper, name);

             self->DefineMemory(name_wrapper, type, location);
           })
      .def("define_function",
           [](OMR::JitBuilder::MethodBuilder *self, const char *name,
              const char *fileName, const char *lineNumber,
              uintptr_t entryPoint, OMR::JitBuilder::IlType *returnType,
              int32_t numParms,
              std::vector<OMR::JitBuilder::IlType *> parmTypes) {
             char *name_wrapper = new char[strlen(name)];
             strcpy(name_wrapper, name);

             char *fileName_wrapper = new char[strlen(fileName)];
             strcpy(fileName_wrapper, fileName);

             char *lineNumber_wrapper = new char[strlen(lineNumber)];
             strcpy(lineNumber_wrapper, lineNumber);

             self->DefineFunction(name_wrapper, fileName_wrapper,
                                  lineNumber_wrapper, (void *)entryPoint,
                                  returnType, numParms, parmTypes.data());
           })
      .def("get_method_name", &OMR::JitBuilder::MethodBuilder::GetMethodName)
      .def("get_next_bytecode_from_worklist",
           &OMR::JitBuilder::MethodBuilder::GetNextBytecodeFromWorklist)
      .def("set_vmstate", &OMR::JitBuilder::MethodBuilder::setVMState)
      .def("allocate_method_builder", &OMR::JitBuilder::allocateMethodBuilder);

  py::class_<OMR::JitBuilder::ThunkBuilder, OMR::JitBuilder::MethodBuilder>(
      m, "ThunkBuilder")
      .def(py::init([](OMR::JitBuilder::TypeDictionary *dict, const char *name,
                       OMR::JitBuilder::IlType *returnType,
                       uint32_t numCalleeParms,
                       std::vector<OMR::JitBuilder::IlType *> calleeParms) {
        char *name_wrapper = new char[strlen(name)];
        strcpy(name_wrapper, name);

        return OMR::JitBuilder::ThunkBuilder(
            dict, name_wrapper, returnType, numCalleeParms, calleeParms.data());
      }))
      .def(py::init<void *>())
      .def("allocate_thunk_builder", &OMR::JitBuilder::allocateThunkBuilder);

  py::class_<OMR::JitBuilder::VirtualMachineOperandArray,
             PyVirtualMachineOperandArray,
             OMR::JitBuilder::VirtualMachineState>(m,
                                                   "VirtualMachineOperandArray")
      .def(py::init<OMR::JitBuilder::MethodBuilder *, int32_t,
                    OMR::JitBuilder::IlType *,
                    OMR::JitBuilder::VirtualMachineRegister *>())
      .def(py::init<OMR::JitBuilder::VirtualMachineOperandArray *>())
      .def(py::init<void *>())
      .def("commit", &OMR::JitBuilder::VirtualMachineOperandArray::Commit)
      .def("reload", &OMR::JitBuilder::VirtualMachineOperandArray::Reload)
      .def("make_copy", &OMR::JitBuilder::VirtualMachineOperandArray::MakeCopy)
      .def("merge_into",
           &OMR::JitBuilder::VirtualMachineOperandArray::MergeInto)
      .def("get", &OMR::JitBuilder::VirtualMachineOperandArray::Get)
      .def("move", &OMR::JitBuilder::VirtualMachineOperandArray::Move)
      .def("set", &OMR::JitBuilder::VirtualMachineOperandArray::Set)
      .def("update_array",
           &OMR::JitBuilder::VirtualMachineOperandArray::UpdateArray)
      .def("allocate_virtual_machine_operand_array",
           &OMR::JitBuilder::allocateVirtualMachineOperandArray);

  py::class_<OMR::JitBuilder::VirtualMachineOperandStack,
             PyVirtualMachineOperandStack,
             OMR::JitBuilder::VirtualMachineState>(m,
                                                   "VirtualMachineOperandStack")
      .def(py::init<OMR::JitBuilder::MethodBuilder *, int32_t,
                    OMR::JitBuilder::IlType *,
                    OMR::JitBuilder::VirtualMachineRegister *>())
      .def(py::init<void *>())
      .def("commit", &OMR::JitBuilder::VirtualMachineOperandStack::Commit)
      .def("reload", &OMR::JitBuilder::VirtualMachineOperandStack::Reload)
      .def("make_copy", &OMR::JitBuilder::VirtualMachineOperandStack::MakeCopy)
      .def("merge_into",
           &OMR::JitBuilder::VirtualMachineOperandStack::MergeInto)
      .def("drop", &OMR::JitBuilder::VirtualMachineOperandStack::Drop)
      .def("dup", &OMR::JitBuilder::VirtualMachineOperandStack::Dup)
      .def("pick", &OMR::JitBuilder::VirtualMachineOperandStack::Pick)
      .def("pop", &OMR::JitBuilder::VirtualMachineOperandStack::Pop)
      .def("push", &OMR::JitBuilder::VirtualMachineOperandStack::Push)
      .def("top", &OMR::JitBuilder::VirtualMachineOperandStack::Top)
      .def("update_stack",
           &OMR::JitBuilder::VirtualMachineOperandStack::UpdateStack)
      .def("allocate_virtual_machine_operand_stack",
           &OMR::JitBuilder::allocateVirtualMachineOperandStack);

  py::class_<OMR::JitBuilder::VirtualMachineRegister, PyVirtualMachineRegister,
             OMR::JitBuilder::VirtualMachineState>(m, "VirtualMachineRegister")
      .def(py::init([](OMR::JitBuilder::IlBuilder *b, const char *localName,
                       OMR::JitBuilder::IlType *pointerToRegisterType,
                       uint32_t adjustByStep,
                       OMR::JitBuilder::IlValue *addressOfRegister) {
        char *localName_wrapper = new char[strlen(localName)];
        strcpy(localName_wrapper, localName);

        return OMR::JitBuilder::VirtualMachineRegister(
            b, localName_wrapper, pointerToRegisterType, adjustByStep,
            addressOfRegister);
      }))
      .def(py::init<void *>())
      .def("commit", &OMR::JitBuilder::VirtualMachineRegister::Commit)
      .def("reload", &OMR::JitBuilder::VirtualMachineRegister::Reload)
      .def("make_copy", &OMR::JitBuilder::VirtualMachineRegister::MakeCopy)
      .def("adjust", py::overload_cast<OMR::JitBuilder::IlBuilder *,
                                       OMR::JitBuilder::IlValue *>(
                         &OMR::JitBuilder::VirtualMachineRegister::Adjust))
      .def("load", &OMR::JitBuilder::VirtualMachineRegister::Load)
      .def("store", &OMR::JitBuilder::VirtualMachineRegister::Store)
      .def("allocate_virtual_machine_register",
           &OMR::JitBuilder::allocateVirtualMachineRegister);

  py::class_<OMR::JitBuilder::VirtualMachineRegisterInStruct,
             PyVirtualMachineRegisterInStruct,
             OMR::JitBuilder::VirtualMachineRegister>(
      m, "VirtualMachineRegisterInStruct")
      .def(py::init([](OMR::JitBuilder::IlBuilder *b, const char *structName,
                       const char *localNameHoldingStructAddress,
                       const char *fieldName, const char *localName) {
        char *structName_wrapper = new char[strlen(structName)];
        strcpy(structName_wrapper, structName);
        char *localNameHoldingStructAddress_wrapper =
            new char[strlen(localNameHoldingStructAddress)];
        strcpy(localNameHoldingStructAddress_wrapper,
               localNameHoldingStructAddress);
        char *fieldName_wrapper = new char[strlen(fieldName)];
        strcpy(fieldName_wrapper, fieldName);
        char *localName_wrapper = new char[strlen(localName)];
        strcpy(localName_wrapper, localName);

        return OMR::JitBuilder::VirtualMachineRegisterInStruct(
            b, structName_wrapper, localNameHoldingStructAddress_wrapper,
            fieldName_wrapper, localName_wrapper);
      }))
      .def(py::init<void *>())
      .def("commit", &OMR::JitBuilder::VirtualMachineRegisterInStruct::Commit)
      .def("reload", &OMR::JitBuilder::VirtualMachineRegisterInStruct::Reload)
      .def("make_copy",
           &OMR::JitBuilder::VirtualMachineRegisterInStruct::MakeCopy)
      .def("allocate_virtual_machine_register_in_struct",
           &OMR::JitBuilder::allocateVirtualMachineRegisterInStruct);
}
