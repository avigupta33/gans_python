from unit_tester import isPassing, raises, runTestsFromEnv
from Quantum import Matrix


def testLenDataFail1() -> bool:
    data = list(range(4))
    return raises(
        lambda: Matrix(data, dims=(1, 5)),
        ValueError, "matrix with dims=(1, 5) cannot be created from 4 value(s)"
    )


def testLenDataFail2() -> bool:
    data = list(range(10))
    return raises(
        lambda: Matrix(data, dims=(3, 3)),
        ValueError, "matrix with dims=(3, 3) cannot be created from 10 value(s)"
    )


def testDataShapeFail1() -> bool:
    return raises(
        lambda: Matrix([
            [1, 0],
            [0, 1]
        ], dims=(4, 1)),
        ValueError, "dims of given 2D array (2, 2) don't match dims specified (4, 1)"
    )


def testCopyConstructor() -> bool:
    x = Matrix(Matrix([
        [1, 0],
        [0, 1]
    ], dims=(2, 2)))
    return isPassing(
        expected=Matrix([
            [1, 0],
            [0, 1]]),
        received=x
    )


def testConsistentSeededRandomGeneration() -> bool:
    m = Matrix.gauss(mu=0, sigma=0.2, dims=(5,), seed=1)
    x = Matrix.gauss(mu=0, sigma=0.2, dims=(5,), seed=1)
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
    mat = Matrix(list(range(1, 10)), dims=(3, 3))
    return isPassing(
        expected=Matrix(list(range(2, 20, 2)), dims=(3, 3)),
        received=mat + mat
    )


def testMatAddFail1() -> bool:
    mat1 = Matrix(list(range(1, 13)), dims=(4, 3))
    mat2 = Matrix(list(range(1, 10)), dims=(3, 3))
    return raises(
        lambda: mat1 + mat2,
        ValueError, "matrices are not the same shape"
    )


def testMatSub1() -> bool:
    mat1 = Matrix(list(range(2, 20, 2)), dims=(3, 3))
    mat2 = Matrix(list(range(3, 30, 3)), dims=(3, 3))
    return isPassing(
        expected=Matrix(list(range(1, 10)), dims=(3, 3)),
        received=mat2 - mat1
    )


def testMatSubFail1() -> bool:
    mat1 = Matrix(list(range(1, 13)), dims=(4, 3))
    mat2 = Matrix(list(range(1, 10)), dims=(3, 3))
    return raises(
        lambda: mat1 - mat2,
        ValueError, "matrices are not the same shape"
    )


def testMatElementMul1() -> bool:
    mat1 = Matrix(list(range(1, 13)), dims=(4, 3))
    return isPassing(
        expected=Matrix(list(range(2, 26, 2)), dims=(4, 3)),
        received=2 * mat1
    )


def testMatNotEq1() -> bool:
    mat1 = Matrix(0, dims=(3, 4))
    mat2 = Matrix(0, dims=(4, 3))
    return isPassing(
        expected=False,
        received=mat1 == mat2
    )


def testMatNotEq2() -> bool:
    mat1 = Matrix(2, dims=(3, 3))
    mat2 = Matrix(1, dims=(3, 3))
    return isPassing(
        expected=False,
        received=mat1 == mat2
    )


def testDataProperty() -> bool:
    mat = Matrix(list(range(1, 10)), dims=(3, 3))
    return not any(a != b for a, b in zip(mat.data, tuple(range(1, 10))))


def testMatMul1() -> bool:
    mat1 = Matrix(3, dims=(4, 6))
    mat2 = Matrix(4, dims=(6, 5))
    return isPassing(
        expected=Matrix(72, dims=(4, 5)),
        received=mat1 @ mat2
    )


def testMatMul2() -> bool:
    mat = Matrix([
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ])
    return isPassing(
        expected=
        Matrix([
            [30, 36, 42],
            [66, 81, 96],
            [102, 126, 150]
        ]),
        received=mat @ mat
    )


def testMatMul3() -> bool:
    mat1 = Matrix([[1, 2, 3],
                   [4, 5, 6],
                   [7, 8, 9],
                   [4, 7, 10]])
    mat2 = Matrix([[2, 4, 6, 8, 10],
                   [7, 6, 8, 9, 1],
                   [0, 3, 4, 19, 8]])

    return isPassing(
        expected=Matrix(
            [[16, 25, 34, 83, 36],
             [43, 64, 88, 191, 93],
             [70, 103, 142, 299, 150],
             [57, 88, 120, 285, 127]]),
        received=mat1 @ mat2
    )

def testMatMul4() -> bool:
    mat1 = Matrix([4, 1, 7, 8, 10], dims = (1,5))
    mat2 = Matrix([16, 18, 3, 1, 14], dims = (5,1))

    return isPassing(
        expected=Matrix([251]),
        received=mat1 @ mat2
    )

def testMatMul5() -> bool:
    mat1 = Matrix([16, 18, 3, 1, 14], dims = (5,1))
    mat2 = Matrix([4, 1, 7, 8, 10], dims = (1,5))

    return isPassing(
        expected=Matrix([
            [ 64,  16, 112, 128, 160],
            [ 72,  18, 126, 144, 180],
            [ 12,   3,  21,  24,  30],
            [  4,   1,   7,   8,  10],
            [ 56,  14,  98, 112, 140]]),
        received=mat1 @ mat2
    )

def testMatMul6() -> bool:
    mat1 = Matrix([[2, 4,],
                   [5,13],
                   [7,16],
                   [1,0],
                   [15, 18],
                   [3, 5]])
    mat2 = Matrix([[3, 1, 15, 88, 12, 13, 225, 22],
                   [4, 5, 29, 79, 26, 39, 956, 12]])

    return isPassing(
        expected=Matrix([[   22,    22,   146,   492,   128,   182,  4274,    92],
       [   67,    70,   452,  1467,   398,   572, 13553,   266],
       [   85,    87,   569,  1880,   500,   715, 16871,   346],
       [    3,     1,    15,    88,    12,    13,   225,    22],
       [  117,   105,   747,  2742,   648,   897, 20583,   546],
       [   29,    28,   190,   659,   166,   234,  5455,   126]]),
        received=mat1 @ mat2
    )

def testMatMulFail1() -> bool:
    mat1 = Matrix(1, dims=(4, 2))
    mat2 = Matrix(4, dims=(6, 5))
    return raises(
        lambda: mat1 @ mat2,
        ValueError, "Matrix 1 has dims (4, 2) while Matrix 2 has dims (6, 5); Incompatible for multiplication."
    )


def testTranspose1() -> bool:
    mat = Matrix([
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ])
    return isPassing(
        expected=
        Matrix([
            [1, 4, 7],
            [2, 5, 8],
            [3, 6, 9]
        ]),
        received=mat.T
    )


def testTranspose2() -> bool:
    mat = Matrix([
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ])
    return isPassing(
        expected=mat,
        received=mat.T.T
    )


def testTranspose3() -> bool:
    data = list(range(5))
    mat = Matrix(data, dims=(5,))
    return isPassing(
        expected=Matrix(data, dims=(1, 5)),
        received=mat.T
    )


def testTranspose4() -> bool:
    data = list(range(1, 9))
    mat = Matrix(data, dims=(2, 4))
    return isPassing(
        expected=Matrix([[1, 5], [2, 6], [3, 7], [4, 8]]),
        received=mat.T
    )


if __name__ == "__main__":
    runTestsFromEnv("Matrix tests", locals())
