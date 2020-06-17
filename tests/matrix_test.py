from unit_tester import *
from matrix import *

def equals(first: 'Matrix', second: 'Matrix') -> bool:
    '''Check equivalence of two matrices'''
    return (first.rows == second.rows and
            first.cols == second.cols and
            not any(a != b for a,b in zip(first.data, second.data)))


def testZeros() -> bool:
    return not any(x != 0 for x in Matrix.zeros(3,3).data)


def testConst() -> bool:
    return not any(x != 5 for x in Matrix.const(3,3,5).data)


def testMatMul1() -> bool:
    mat1 = Matrix.const(rows=4, cols=6, val=3)
    mat2 = Matrix.const(rows=6, cols=5, val=4)
    expected = Matrix.const(rows=4, cols=5, val=72)
    received = mat1 @ mat2
    return equals(expected, received)


def testMatMul2() -> bool:
    mat = Matrix(3,3,[
        1,2,3,
        4,5,6,
        7,8,9
    ])
    expected = Matrix(3,3,[
        30,36,42,
        66,81,96,
        102,126,150
    ])
    received = mat @ mat
    return equals(expected, received)

def testMatAdd1() -> bool:
    mat = Matrix(3, 3, [
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    ])
    expected = Matrix(3, 3, [
        2, 4, 6,
        8, 10, 12,
        14, 16, 18
    ])
    received = mat + mat
    return equals(expected, received)


if __name__  ==  "__main__":
    runTests("Matrix tests", [
        testZeros,
        testConst,
        testMatMul1,
        testMatMul2,
        testMatAdd1
    ])