import jitbuilder
import ctypes
from typing import List, NoReturn, NewType
import numpy as np
from numpy.ctypeslib import ndpointer

initialized = jitbuilder.initialize_jit()
if not initialized:
    print("failed to init")
    exit()

int32 = NewType("int32", int)
int32.__name__ = "int32"
int64 = NewType("int64", int)
int64.__name__ = "int64"
float32 = NewType("float32", float)
float32.__name__ = "float32"
float64 = NewType("float64", float)
float64.__name__ = "float64"
array_int32 = NewType("array_int32", List[int])
array_int32.__name__ = "array_int32"
array_int64 = NewType("array_int64", List[int])
array_int64.__name__ = "array_int64"
array_float32 = NewType("array_float32", List[float])
array_float32.__name__ = "array_float32"
array_float64 = NewType("float64", List[float])
array_float64.__name__ = "array_float64"
array2d_int32 = NewType("array2d_int32", List[List[int]])
array2d_int32.__name__ = "array2d_int32"
array2d_int64 = NewType("array2d_int64", List[List[int]])
array2d_int64.__name__ = "array2d_int64"
array2d_float32 = NewType("array2d_float32", List[List[float]])
array2d_float32.__name__ = "array2d_float32"
array2d_float64 = NewType("array2d_float64", List[List[float]])
array2d_float64.__name__ = "array2d_float64"
void = NewType("void", NoReturn)
void.__name__ = "void"


array2d_as_uintp = ndpointer(dtype=np.uintp, ndim=1, flags=["C"])


type_dict = jitbuilder.TypeDictionary()

jitbuilder_array_int32 = type_dict.p_int_32
jitbuilder_array2d_int32 = type_dict.p_int_32
jitbuilder_array_int64 = type_dict.p_int_64
jitbuilder_array2d_int64 = type_dict.p_int_64
jitbuilder_array_float32 = type_dict.p_float
jitbuilder_array2d_float32 = type_dict.p_float
jitbuilder_array_float64 = type_dict.p_double
jitbuilder_array2d_float64 = type_dict.p_double


type_map_jitbuilder = {
    "int32": type_dict.int_32,
    "int64": type_dict.int_64,
    "float32": type_dict.float,
    "float64": type_dict.double,
    "array_int64": jitbuilder_array_int64,
    "array_int32": jitbuilder_array2d_int32,
    "array_float32": jitbuilder_array_float32,
    "array_float64": jitbuilder_array_float64,
    "array2d_float32": jitbuilder_array2d_float32,
    "array2d_float64": jitbuilder_array2d_float64,
    "array2d_int64": jitbuilder_array2d_int64,
    "array2d_int32": jitbuilder_array2d_int32,
    "void": type_dict.no_type,
}

type_map_ctypes = {
    "int32": ctypes.c_int32,
    "int64": ctypes.c_int64,
    "float32": ctypes.c_float,
    "float64": ctypes.c_double,
    "array_int32": ctypes.POINTER(ctypes.c_int32),
    "array_int64": ctypes.POINTER(ctypes.c_int64),
    "array_float32": ctypes.POINTER(ctypes.c_float),
    "array_float64": ctypes.POINTER(ctypes.c_double),
    "array2d_int64": array2d_as_uintp,
    "array2d_float64": array2d_as_uintp,
    "void": ctypes.c_voidp,
}

type_map_numpy = {
    "i": ctypes.c_int,
    "f": ctypes.c_float,
    "d": ctypes.c_double,
    "l": ctypes.c_long,
}


def to_jitbuilder_type(from_type):
    return type_map_jitbuilder[from_type]


def to_ctype(type_):
    return type_map_ctypes[type_]


def is_array(var_type):
    return var_type.startswith("array")