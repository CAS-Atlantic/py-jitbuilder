import pyperf
import numpy as np

runner = pyperf.Runner()


N_dot_product = [10, 100, 1000, 1000000, 100000000]
setup_dot_product = """
from __main__ import dot
import numpy as np
N = {N}
A = np.random.randint(0, 1000, size=N)
B = np.random.randint(0, 1000, size=N)

dot(A,B,N)
"""

stmt_dot_product = """
dot(A,B,N)
"""


def dot(a, b, N):
    c = 0
    for i in range(N):
        c += a[i] * b[i]
    return c


for N in N_dot_product:
    runner.timeit(
        f"dot_product_pypy-{N}",
        stmt=stmt_dot_product,
        setup=setup_dot_product.format(N=N),
    )

N_fib = [10, 30, 50]
setup_iterative_fib = """
from __main__ import iterative_fib
N = {N}
iterative_fib(N)
"""

stmt_iterative_fib = """
iterative_fib(N)
"""


def iterative_fib(n):
    a = 0
    b = 1
    for i in range(n):
        temp = a
        a = b
        b = temp + b
    return a


for N in N_fib:
    runner.timeit(
        f"iterative_fib_pypy-{N}",
        stmt=stmt_iterative_fib,
        setup=setup_iterative_fib.format(N=N),
    )


setup_recursive_fib = """
from __main__ import recursive_fib
N = {N}

recursive_fib(N)
"""

stmt_recursive_fib = """
recursive_fib(N)
"""


def recursive_fib(n):
    if n == 0:
        return 0
    elif n == 1:
        return 1
    else:
        return recursive_fib(n - 1) + recursive_fib(n - 2)


for N in N_fib:

    runner.timeit(
        f"recursive_fib_pypy-{N}",
        stmt=stmt_recursive_fib,
        setup=setup_recursive_fib.format(N=N),
    )

N_matrix_multiplication = [10, 100]
setup_matrix_multiplication = """
from __main__ import matrix_multiplication
import numpy as np
N = {N}
C = np.zeros((N, N))
A = np.random.rand(N, N)
B = np.random.rand(N, N)


matrix_multiplication(C, A, B, N)
"""

stmt_matrix_multiplication = """
matrix_multiplication(C, A, B, N)
"""


def matrix_multiplication(c, a, b, n):
    for i in range(n):
        for j in range(n):
            for k in range(n):
                c[i][j] = c[i][j] + (a[i][k] * b[k][j])


for N in N_matrix_multiplication:
    runner.timeit(
        f"matrix_multiplication_pypy-{N}",
        stmt=stmt_matrix_multiplication,
        setup=setup_matrix_multiplication.format(N=N),
    )
