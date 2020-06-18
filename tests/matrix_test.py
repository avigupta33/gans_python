
'''
Matrix testing script
'''

from unit_tester import *
from matrix import *


def testZeros() -> bool:
    return not any(x != 0 for x in Matrix.zeros(3,3).data)


def testConst() -> bool:
    return not any(x != 5 for x in Matrix.const(3,3,5).data)


def testConstNegDimFail1() -> bool:
    return raises(
        lambda: Matrix.const(rows=-1, cols=3, val=4),
        ValueError,
        "Rows must be positive, is -1"
    )


def testMatMul1() -> bool:
    mat1 = Matrix.const(rows=4, cols=6, val=3)
    mat2 = Matrix.const(rows=6, cols=5, val=4)
    expected = Matrix.const(rows=4, cols=5, val=72)
    received = mat1 @ mat2
    return expected == received


def testMatMul2() -> bool:
    mat = Matrix(3,3,[
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    ])
    expected = Matrix(3,3,[
        30, 36, 42,
        66, 81, 96,
        102, 126, 150
    ])
    received = mat @ mat
    return expected == received


def testMatMulFail1() -> bool:
    mat1 = Matrix.const(rows=4, cols=6, val=3)
    mat2 = Matrix.const(rows=6, cols=5, val=4)
    return raises(
        lambda: mat2 @ mat1, 
        ValueError, 
        "Matrix A has dims (6, 5) while Matrix B has dims (4, 6). Incompatible for multiplication"
    )


def testMatAdd1() -> bool:
    mat = Matrix(3,3,[
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    ])
    expected = Matrix(3,3,[
        2, 4, 6,
        8, 10, 12,
        14, 16, 18
    ])
    received = mat + mat
    return expected == received


def testMatAddFail1() -> bool:
    mat1 = Matrix.const(rows=4, cols=6, val=3)
    mat2 = Matrix.const(rows=6, cols=5, val=4)
    return raises(
        lambda: mat1 + mat2,
        ValueError,
        "Matrix A has dims (4, 6) while Matrix B has dims (6, 5). Incompatible for addition"
    )


def testTranspose1() -> bool:
    mat = Matrix(3,3,[
        1,2,3,
        4,5,6,
        7,8,9
    ])
    expected = Matrix(3,3,[
        1,4,7,
        2,5,8,
        3,6,9
    ])
    received = mat.t
    return expected == received


if __name__  ==  "__main__":
    runTests("Matrix tests", [
        testZeros,
        testConst,
        testConstNegDimFail1,
        testMatMul1,
        testMatMul2,
        testMatMulFail1,
        testMatAdd1,
        testMatAddFail1,
        testTranspose1
    ])