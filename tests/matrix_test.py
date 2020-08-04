'''
Matrix testing script
'''

# from c_extensions import Quantum as qa
from c_extensions.Quantum import Matrix, seed
from unit_tester import isPassing, raises, runTestsFromEnv


def testLenDataFail1() -> bool:
    data = list(range(4))
    return raises(
        lambda: Matrix(data, dims = (1,5)),
        ValueError, "matrix with dims=(1, 5) cannot be created from 4 value(s)"
    )


def testLenDataFail2() -> bool:
    data = list(range(10))
    return raises(
        lambda: Matrix(data, dims = (3,3)),
        ValueError, "matrix with dims=(3, 3) cannot be created from 10 value(s)"
    )

def testDataShapeFail1() -> bool:
    return raises(
        lambda: Matrix([[1, 0], [0, 1]], dims=(4, 1)),
        ValueError, "dims of given 2D array (2, 2) don't match dims specified (4, 1)"
    )

def testCopyConstructor() -> bool:
    x = Matrix(Matrix([[1, 0], [0, 1]], dims=(2, 2)))
    return isPassing(
        expected = Matrix([[1, 0], [0, 1]], dims=(2, 2)),
        received = x
    )

def testConsistentRandomGeneration() -> bool:
    seed(1)
    m = Matrix.gauss(mu=0, sigma=0.2, dims=(5,))
    x = Matrix.gauss(mu=0, sigma=0.2, dims=(5,))
    return isPassing(
        expected=m,
        received=x
    )

def testNegDimFail1() -> bool:
    return raises(
        lambda: Matrix([[1, 0], [0, 1]], dims=(-1, 2)),
        ValueError, "dims must be positive, non-zero values: (-1, 2)"
    )

def testMatAdd1() -> bool:
    mat = Matrix(list(range(1,10)), dims = (3,3))
    return isPassing(
        expected = Matrix(list(range(2,20,2)), dims = (3,3)),
        received = mat + mat
    )

def testMatAddFail1() -> bool:
    mat1 = Matrix(list(range(1, 13)), dims=(4, 3))
    mat2 = Matrix(list(range(1,10)), dims = (3,3))
    return raises(
        lambda: mat1 + mat2,
        ValueError, "matrices are not the same shape"
    )

def testMatSub1() -> bool:
    mat1 = Matrix(list(range(2,20,2)), dims = (3,3))
    mat2 = Matrix(list(range(3,30,3)), dims = (3,3))
    return isPassing(
        expected = Matrix(list(range(1,10)), dims = (3,3)),
        received = mat2 - mat1
    )

def testMatSubFail1() -> bool:
    mat1 = Matrix(list(range(1, 13)), dims=(4,3))
    mat2 = Matrix(list(range(1, 10)), dims = (3,3))
    return raises(
        lambda: mat1 - mat2,
        ValueError, "matrices are not the same shape"
    )

def testMatElementMul1() -> bool:
    mat1 = Matrix(list(range(1, 13)), dims=(4, 3))
    return isPassing(
        expected = Matrix(list(range(2, 26, 2)), dims=(4, 3)),
        received = 2 * mat1
    )


def testMatNotEq1() -> bool:
    mat1 = Matrix([0] * 12, dims = (3,4))
    mat2 = Matrix([0] * 12, dims = (4,3))
    return isPassing(
        expected = False,
        received = mat1 == mat2
    )


def testMatNotEq2() -> bool:
    mat1 = Matrix([2] * 9, dims=(3, 3))
    mat2 = Matrix([1] * 9, dims=(3, 3))
    return isPassing(
        expected = False,
        received = mat1 == mat2
    )

def testDataProperty() -> bool:
    mat = Matrix(list(range(1, 10)), dims=(3, 3))
    return not any(a!=b for a,b in zip(mat.data, tuple(range(1, 10))))

def testMatMul1() -> bool:
    mat1 = Matrix([3] * 24, dims  = (4, 6))
    mat2 = Matrix([4] * 30, dims = (6, 5))
    return isPassing(
        expected = Matrix([72] * 20, dims = (4, 5)),
        received = mat1 @ mat2
    )

def testMatMul2() -> bool:
    mat = Matrix([
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    ], dims = (3,3))
    return isPassing(
        expected =
            Matrix([
                30, 36, 42,
                66, 81, 96,
                102, 126, 150
            ], dims = (3,3)),
        received = mat @ mat
    )


def testMatMulFail1() -> bool:
    mat1 = Matrix(1, dims=(4, 2))
    mat2 = Matrix(4, dims=(6,5))
    return raises(
        lambda: mat1 @ mat2,
        ValueError, "Matrix 1 has dims (4, 2) while Matrix 2 has dims (6, 5); Incompatible for multiplication."
    )


def testTranspose1() -> bool:
    mat = Matrix([
        1,2,3,
        4,5,6,
        7,8,9
    ], dims = (3,3))
    return isPassing(
        expected =
            Matrix([
                1,4,7,
                2,5,8,
                3,6,9
            ], dims = (3,3)),
        received = mat.T
    )

def testTranspose2() -> bool:
    mat = Matrix([
        1,2,3,
        4,5,6,
        7,8,9
    ], dims = (3,3))
    return isPassing(
        expected = mat,
        received = mat.T.T
    )

if __name__  ==  "__main__":
    # runTestsFromEnv() should be safe when __name__ == "__main__"
    runTestsFromEnv("Matrix tests", locals())

