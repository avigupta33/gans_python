from Quantum import *
from time import time


def display(mat, tabspace=3):
    for row_i in range(mat.rows):
        print('\t'.join(str(mat.data[row_i * mat.cols + col_i]) for col_i in range(mat.cols)).expandtabs(tabspace))
    print()


def timer(func, iters=10):
    dim = 100
    mean = sum(func(dim) for _ in range(iters))/iters
    print(f"C ({dim},{dim}) @ ({dim},{dim})")
    print(f"Mean execution time of {iters} runs: {mean}")


def matmulTest(dim):
    mat1 = Matrix.array(rows=dim, cols=dim, data=[x for x in range(10000)])
    mat2 = Matrix.array(rows=dim, cols=dim, data=[x for x in range(10000)])
    ts = time()
    mat2 @ mat1
    te = time()
    return te - ts
    

def demo():
    # Make a matrix from 2D array
    print("Matrix from 2D")
    matFrom2D = Matrix([
        [1,2,3],
        [4,5,6],
        [7,8,9]
    ])
    display(matFrom2D)

    # Make a matrix of zeros
    print("Matrix from zeros")
    matFromZero = Matrix.zeros(rows=3, cols=3)
    display(matFromZero)

    # Make a matrix of scalars
    print("Matrix from fill")
    matFromFill = Matrix.fill(rows=3, cols=3, val=12)
    display(matFromFill)

    # Make a matrix from a flattened array
    print("Matrix from 1D")
    matFrom1D = Matrix.array(rows=3, cols=3, data=[x for x in range(10,19)])
    display(matFrom1D)

    # Make a matrix from Gaussian random distribution
    print("Matrix from gauss")
    matFromGauss = Matrix.gauss(rows=3, cols=3, mu=0, sigma=1)
    display(matFromGauss)

    # Make a matrix from uniform random distribution
    print("Matrix from uniform")
    matFromUniform = Matrix.uniform(rows=3, cols=3, lower_bound=-1, upper_bound=1)
    display(matFromUniform)

    # timer(matmulTest)
    # pass
    # dim = 5
    # mat1 = Matrix(rows=dim, cols=dim, data=[x for x in range(1,1+dim*dim)])
    # display(mat1/10)
    # twod = Matrix([[1,2],[3,4]])
    # display(twod)
    # print()
    # zeros = Matrix.array(4, 4, [x for x in range(16)])
    # display(zeros @ zeros.T)
    # print()
    # display(mat1)
    # fives = q.fill(4, 1, 5)
    # display(fives)
    # gauss = Matrix.gauss(rows=5, cols=1, mu=0, sigma=1)
    # display(gauss)
    # print()
    # uniform = Matrix.uniform(rows=5, cols=1, lower_bound=0, upper_bound=1)
    # display(uniform)
    # print(mat1* 8)
    # display(mat1*2)
    # display(mat1*1.5)
    # display(2*mat1)
    # display(1.5*mat1)


    add = matFrom1D + matFrom2D
    sub = matFrom1D - matFrom2D
    mul = matFrom1D * matFrom2D
    matmul = matFrom1D @ matFrom2D
    print("Addition:")
    display(add)
    print("Subtraction:")
    display(sub)
    print("Multiply:")
    display(mul)
    print("Matrix Multiply:")
    display(matmul)

    relud = tanh_backwards(matFromUniform)
    print("ReLU of uniform")
    display(relud)


if __name__ == "__main__":
    demo()
    timer(matmulTest, iters=100)



