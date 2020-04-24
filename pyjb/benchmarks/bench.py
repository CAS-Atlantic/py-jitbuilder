import jitbuilder
import pyperf
from numba import njit, jit
import numpy as np
from pyjb.pyjbtypes import *
import ctypes
from pyjb.jitbuildermethod import jitbuilder_func


runner = pyperf.Runner()


# N_dot_product = [10, 100, 1000, 1000000, 100000000]
# setup_dot_product = """
# from __main__ import {fn_prefix}_dot
# from numba import njit
# import numpy as np
# import ctypes
# N = {N}
# A = np.random.randint(0, 1000, size=N)
# B = np.random.randint(0, 1000, size=N)

# a = A.ctypes.data_as(ctypes.POINTER(ctypes.c_int64))
# b = B.ctypes.data_as(ctypes.POINTER(ctypes.c_int64))

# {fn_prefix}_dot({params})
# """

# stmt_dot_product = """
# {}_dot({})
# """


# def python_dot(a: array_int64, b: array_int64, N: int64) -> int64:
#     c: int64 = 0
#     for i in range(N):
#         c += a[i] * b[i]
#     return c


# jitbuilder_dot = jitbuilder_func(python_dot)
# numba_dot = njit(python_dot)

# for N in N_dot_product:
#     runner.timeit(
#         f"dot_product_python_jitbuilder-{N}",
#         stmt=stmt_dot_product.format("jitbuilder", "a, b, N"),
#         setup=setup_dot_product.format(N=N, fn_prefix="jitbuilder", params="a, b, N"),
#     )
#     runner.timeit(
#         f"dot_product_python_numba-{N}",
#         stmt=stmt_dot_product.format("numba", "A, B, N"),
#         setup=setup_dot_product.format(N=N, fn_prefix="numba", params="A, B, N"),
#     )
#     runner.timeit(
#         f"dot_product_python-{N}",
#         stmt=stmt_dot_product.format("python", "A, B, N"),
#         setup=setup_dot_product.format(N=N, fn_prefix="python", params="A, B, N"),
#     )

N_fib = [10, 30, 50]
# setup_iterative_fib = """
# from __main__ import {fn_prefix}_iterative_fib
# N = {N}
# {fn_prefix}_iterative_fib(N)
# """

# stmt_iterative_fib = """
# {}_iterative_fib(N)
# """

# def python_iterative_fib(n: int64) -> int64:
#     a: int64 = 0
#     b: int64 = 1
#     for i in range(n):
#         temp: int64 = a
#         a = b
#         b = temp + b
#     return a

# jitbuilder_iterative_fib = jitbuilder_func(python_iterative_fib)
# numba_iterative_fib = njit(python_iterative_fib)

# for N in N_fib:
#     runner.timeit(
#         f"iterative_fib_python_jitbuilder-{N}",
#         stmt=stmt_iterative_fib.format("jitbuilder"),
#         setup=setup_iterative_fib.format(N=N, fn_prefix="jitbuilder"),
#     )
#     runner.timeit(
#         f"iterative_fib_python_numba-{N}",
#         stmt=stmt_iterative_fib.format("numba"),
#         setup=setup_iterative_fib.format(N=N, fn_prefix="numba"),
#     )
#     runner.timeit(
#         f"iterative_fib_python-{N}",
#         stmt=stmt_iterative_fib.format("python"),
#         setup=setup_iterative_fib.format(N=N, fn_prefix="python"),
#     )


setup_recursive_fib = """
from __main__ import {fn_prefix}_recursive_fib
N = {N}

{fn_prefix}_recursive_fib(N)
"""

stmt_recursive_fib = """
{}_recursive_fib(N)
"""

def python_recursive_fib(n: int64) -> int64:
    if n == 0: return 0
    elif n == 1: return 1
    else: return python_recursive_fib(n-1) + python_recursive_fib(n-2)

jitbuilder_recursive_fib = jitbuilder_func(python_recursive_fib)
numba_recursive_fib = (njit("int64(int64)"))(python_recursive_fib)

for N in N_fib:
    runner.timeit(
        f"recursive_fib_python_jitbuilder-{N}",
        stmt=stmt_recursive_fib.format("jitbuilder"),
        setup=setup_recursive_fib.format(N=N, fn_prefix="jitbuilder"),
    )
    runner.timeit(
        f"recursive_fib_python_numba-{N}",
        stmt=stmt_recursive_fib.format("numba"),
        setup=setup_recursive_fib.format(N=N, fn_prefix="numba"),
    )
    runner.timeit(
        f"recursive_fib_python-{N}",
        stmt=stmt_recursive_fib.format("python"),
        setup=setup_recursive_fib.format(N=N, fn_prefix="python"),
    )

# N_matrix_multiplication = [10, 100, 1000, 2500]
# setup_matrix_multiplication = """
# from __main__ import {fn_prefix}_matrix_multiplication
# import numpy as np
# N = {N}
# C = np.zeros((N, N))
# A = np.random.rand(N, N)
# B = np.random.rand(N, N)

# a = (A.__array_interface__["data"][0]+ np.arange(A.shape[0]) * A.strides[0]).astype(np.uintp)
# b = (B.__array_interface__["data"][0]+ np.arange(B.shape[0]) * B.strides[0]).astype(np.uintp)
# c = (C.__array_interface__["data"][0]+ np.arange(C.shape[0]) * C.strides[0]).astype(np.uintp)

# {fn_prefix}_matrix_multiplication({params})
# """

# stmt_matrix_multiplication = """
# {}_matrix_multiplication({})
# """


# def python_matrix_multiplication(
#     c: array2d_float64, a: array2d_float64, b: array2d_float64, n: int64
# ) -> void:
#     for i in range(n):
#         for j in range(n):
#             for k in range(n):
#                 c[i][j] = c[i][j] + (a[i][k] * b[k][j])


# jitbuilder_matrix_multiplication = jitbuilder_func(python_matrix_multiplication)
# numba_matrix_multiplication = njit(python_matrix_multiplication)

# for N in N_matrix_multiplication:
#     runner.timeit(
#         f"matrix_multiplication_python_jitbuilder-{N}",
#         stmt=stmt_matrix_multiplication.format("jitbuilder", "c, a, b, N"),
#         setup=setup_matrix_multiplication.format(
#             N=N, fn_prefix="jitbuilder", params="c, a, b, N"
#         ),
#     )
#     runner.timeit(
#         f"matrix_multiplication_python_numba-{N}",
#         stmt=stmt_matrix_multiplication.format("numba", "C, A, B, N"),
#         setup=setup_matrix_multiplication.format(
#             N=N, fn_prefix="numba", params="C, A, B, N"
#         ),
#     )
#     if N < 1000:
#         runner.timeit(
#             f"matrix_multiplication_python-{N}",
#             stmt=stmt_matrix_multiplication.format("python", "C, A, B, N"),
#             setup=setup_matrix_multiplication.format(
#                 N=N, fn_prefix="python", params="C, A, B, N"
#             ),
#         )


# setup_vector_matrix_multiplication = """
# from __main__ import vec_mat_mul_cfunc
# import numpy as np
# import ctypes
# N = {N}
# C = np.zeros((N, N))
# A = np.random.rand(N, N)
# B = np.random.rand(N, N)
# a = A.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
# b = B.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
# c = C.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

# vec_mat_mul_cfunc(c,a,b,N)
# """

# stmt_vector_matrix_multiplication = """
# vec_mat_mul_cfunc(c,a,b,N)
# """


# class VectorMatMul(jitbuilder.MethodBuilder):
#     def __init__(self, types):
#         jitbuilder.MethodBuilder.__init__(self, types)
#         self.define_file(__file__)
#         self.define_name("vecmatmul")
#         self.td = types

#         self.p_double = types.pointer_to(types.double)

#         self.define_parameter("C", self.p_double)
#         self.define_parameter("A", self.p_double)
#         self.define_parameter("B", self.p_double)
#         self.define_parameter("N", self.int_32)

#         self.define_return_type(self.no_type)

#         self.define_local("sum", self.vector_double)

#     def vector_store_2d(self, bldr, base, first, second, N, value):
#         bldr.vector_store_at(
#             bldr.index_at(self.p_double, base, bldr.add(bldr.mul(first, N), second)),
#             value,
#         )

#     def vector_load_2d(self, bldr, base, first, second, N):
#         return bldr.vector_load_at(
#             self.p_double,
#             bldr.index_at(self.p_double, base, bldr.add(bldr.mul(first, N), second)),
#         )

#     def load_2d(self, bldr, base, first, second, N):
#         return bldr.load_at(
#             self.p_double,
#             bldr.index_at(self.p_double, base, bldr.add(bldr.mul(first, N), second)),
#         )

#     def buildIL(self):
#         self.all_locals_have_been_defined()
#         A = self.load("A")
#         B = self.load("B")
#         C = self.load("C")
#         N = self.load("N")

#         zero = self.const_int_32(0)
#         one = self.const_int_32(1)
#         two = self.const_int_32(2)

#         iloop = None
#         jloop = None
#         kloop = None

#         iloop = self.for_loop_up("i", None, zero, N, one)
#         i = iloop.load("i")

#         # vectorizing loop
#         jloop = iloop.for_loop_up("j", None, zero, N, two)
#         j = jloop.load("j")

#         jloop.vector_store("sum", jloop.const_double(0.0))

#         kloop = jloop.for_loop_up("k", None, zero, N, one)
#         k = kloop.load("k")

#         A_ik = self.load_2d(kloop, A, i, k, N)
#         B_kj = self.vector_load_2d(kloop, B, k, j, N)

#         kloop.vector_store(
#             "sum", kloop.add(kloop.vector_load("sum"), kloop.mul(A_ik, B_kj))
#         )

#         self.vector_store_2d(jloop, C, i, j, N, jloop.load("sum"))

#         self.return_()

#         return True


# vec_types = jitbuilder.TypeDictionary()
# CFUNC = ctypes.CFUNCTYPE(
#     None,
#     ctypes.POINTER(ctypes.c_double),
#     ctypes.POINTER(ctypes.c_double),
#     ctypes.POINTER(ctypes.c_double),
#     ctypes.c_int64,
# )


# vec_mat_mul = VectorMatMul(vec_types)
# vec_mat_mul_func = jitbuilder.compile_method_builder(vec_mat_mul)

# ctypes.pythonapi.PyCapsule_GetPointer.restype = ctypes.c_void_p
# vec_mat_mul_cfunc = CFUNC(
#     ctypes.pythonapi.PyCapsule_GetPointer(ctypes.py_object(vec_mat_mul_func), 0)
# )

# for N in N_matrix_multiplication:
#     runner.timeit(
#         f"matrix_multiplication_python_jitbuilder-{N}",
#         stmt=stmt_vector_matrix_multiplication,
#         setup=setup_vector_matrix_multiplication.format(N=N),
#     )