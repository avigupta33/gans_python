'''
Matrix testing script
'''

from unit_tester import *
from gans_types import *


def testLenDataFail1() -> bool:
    data = [0,1,2,3]
    return raises(
        lambda: Matrix(rows=1, cols=5, data=data),
        ValueError, "Expected 5 elements, but received 4 elements"
    )


def testLenDataFail2() -> bool:
    data = [0,1,2,3,4,5,6,7,8,9]
    return raises(
        lambda: Matrix(rows=3, cols=3, data=data),
        ValueError, "Expected 9 elements, but received 10 elements"
    )


def testZeros() -> bool:
    return not any(x != 0 for x in Matrix.zeros(3,3).unordered_data)


def testConst() -> bool:
    return not any(x != 5 for x in Matrix.const(3,3,5).unordered_data)


def testConstNegDimFail1() -> bool:
    return raises(
        lambda: Matrix.const(rows=-1, cols=3, val=4),
        ValueError, "Rows must be positive, is -1"
    )


def testRepr1() -> bool:
    mat = Matrix.zeros(rows=5, cols=5)
    return isPassing(
        expected = "Matrix with 5 rows and 5 cols", 
        received = str(mat)
    )


def testLen1() -> bool:
    mat = Matrix.zeros(rows=4, cols=9)
    return isPassing(
        expected = 36,
        received = len(mat)
    )


def testCompress1()-> bool:
    mat1 = Matrix.const(rows=4, cols=3, val=3)
    mat2 = Matrix.const(rows=4, cols=3, val=5)
    return isPassing(
        expected = Matrix.const(rows=4, cols=3, val=8),
        received = Matrix.compress(mat1, mat2, lambda a,b: a+b)
    )


def testCompress2()-> bool:
    mat1 = Matrix(rows=3, cols=3, data=[
        1,-6,9,
        -3,-4,4,
        8,2,0
    ])
    mat2 = Matrix.zeros(rows=3, cols=3)
    return isPassing(
        expected = Matrix(rows=3, cols=3, data=[
            1,0,9,
            0,0,4,
            8,2,0
        ]),
        received = Matrix.compress(mat1, mat2, lambda a,b: max(a,b))
    )


def testMatAdd1() -> bool:
    mat = Matrix(rows=3, cols=3, data=[
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    ])
    return isPassing(
        expected = 
            Matrix(rows=3, cols=3, data=[
                2, 4, 6,
                8, 10, 12,
                14, 16, 18
            ]),
        received = mat + mat
    )

def testMatAddFail1() -> bool:
    mat1 = Matrix.const(rows=4, cols=6, val=3)
    mat2 = Matrix.const(rows=6, cols=5, val=4)
    return raises(
        lambda: mat1 + mat2,
        ValueError, "Matrix 1 has dims (4, 6) while Matrix 2 has dims (6, 5). Incompatible for folding"
    )


def testMatSub1() -> bool:
    mat1 = Matrix.const(rows=4, cols=5, val=12)
    mat2 = Matrix.const(rows=4, cols=5, val=9)
    return isPassing(
        expected = Matrix.const(rows=4, cols=5, val=3),
        received = mat1 - mat2
    )


def testMatElementMul1() -> bool:
    mat = Matrix(rows=5, cols=1, data=[1,2,3,4,5])
    return isPassing(
        expected = Matrix(rows=5, cols=1, data=[1,4,9,16,25]),
        received = mat * mat
    )


def testMatNotEq1() -> bool:
    mat1 = Matrix.zeros(rows=3, cols=4)
    mat2 = Matrix.zeros(rows=4, cols=3)
    return isPassing(
        expected = False,
        received = mat1 == mat2
    )


def testMatNotEq2() -> bool:
    mat1 = Matrix.zeros(rows=4, cols=3)
    mat2 = Matrix.zeros(rows=3, cols=4)
    return isPassing(
        expected = False,
        received = mat1 == mat2
    )


def testDataProperty() -> bool:
    mat = Matrix(rows=3, cols=3, data=[
        1,2,3,
        4,5,6,
        7,8,9
    ])
    return not any(a!=b for a,b in zip(mat.data, mat.unordered_data))


def testManyIterRowCalls() -> bool:
    mat = Matrix.zeros(rows=3, cols=3)
    for _ in range(10):
        for row in mat.iterRow():
            if len(list(row())) != 3:
                return False
    return True


def testManyIterColCalls() -> bool:
    mat = Matrix.zeros(rows=3, cols=3)
    for _ in range(10):
        for col in mat.iterCol():
            if len(list(col())) != 3:
                return False
    return True


def testMatMul1() -> bool:
    mat1 = Matrix.const(rows=4, cols=6, val=3)
    mat2 = Matrix.const(rows=6, cols=5, val=4)
    return isPassing(
        expected = Matrix.const(rows=4, cols=5, val=72),
        received = mat1 @ mat2
    )


def testMatMul2() -> bool:
    mat = Matrix(rows=3, cols=3, data=[
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    ])
    return isPassing(
        expected = 
            Matrix(rows=3, cols=3, data=[
                30, 36, 42,
                66, 81, 96,
                102, 126, 150
            ]),
        received = mat @ mat
    )


def testMatMulFail1() -> bool:
    mat1 = Matrix.const(rows=4, cols=6, val=3)
    mat2 = Matrix.const(rows=6, cols=5, val=4)
    return raises(
        lambda: mat2 @ mat1, 
        ValueError, "Matrix A has dims (6, 5) while Matrix B has dims (4, 6). Incompatible for multiplication"
    )


def testTranspose1() -> bool:
    mat = Matrix(rows=3, cols=3, data=[
        1,2,3,
        4,5,6,
        7,8,9
    ])
    return isPassing(
        expected = 
            Matrix(rows=3, cols=3, data=[
                1,4,7,
                2,5,8,
                3,6,9
            ]),
        received = mat.t
    )
   

def testTranspose2() -> bool:
    mat = Matrix(rows=3, cols=3, data=[
        1,2,3,
        4,5,6,
        7,8,9
    ])
    return isPassing(
        expected = mat,
        received = mat.t.t
    )


def testGenerate1() -> bool:
    return isPassing(
        expected = Matrix.zeros(rows=3, cols=5),
        received = Matrix.generate(rows=3, cols=5, generator=lambda: 0)
    )


if __name__  ==  "__main__":
    # runTestsFromEnv() should be safe when __name__ == "__main__"
    runTestsFromEnv("Matrix tests", locals())

