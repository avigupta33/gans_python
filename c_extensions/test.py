from Quantum import *
import matplotlib.pyplot as plt
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

    print("Addition:")
    display(matFrom1D + matFrom2D)
    print("Subtraction:")
    display(matFrom1D - matFrom2D)
    print("Multiply:")
    display(matFrom1D * matFrom2D)
    print("Matrix Multiply:")
    display(matFrom1D @ matFrom2D)

    activated = tanh_forwards(matFromUniform)
    print("Activated of uniform")
    display(activated)


def activationDemo():
    upper_bound = 50
    lower_bound = -upper_bound
    x_points = list(range(lower_bound, upper_bound))
    ranged = Matrix.array(rows=(upper_bound-lower_bound), cols=1, data=x_points)
    activated = sigmoid_forwards(ranged)
    plt.scatter(x_points, list(activated.data))
    plt.show()


def histogramDemo():
    mat_gauss = Matrix.gauss(rows=1000000, cols=1, mu=0, sigma=0.2)
    x_points = list(mat_gauss.data)
    print("Made x_points")
    plt.hist(x_points, density=True, bins=30)
    plt.ylabel("Frequency")
    plt.xlabel("Randomly generated number")
    plt.show()

if __name__ == "__main__":
    # demo()
    # activationDemo()
    histogramDemo()
    timer(matmulTest, iters=100)



