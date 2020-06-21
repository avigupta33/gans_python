from Quantum import Matrix

def display(mat, tabspace=4):
    r, c, data = mat.rows, mat.cols, mat.data
    index = 0
    for row_i in range(r):
        print('\t'.join(str(data[row_i * c + col_i]) for col_i in range(c)).expandtabs(tabspace))

from time import time

def timer(func, iters=10):
    dim = 100
    mean = sum(func(dim) for _ in range(iters))/iters
    print(f"C ({dim},{dim}) @ ({dim},{dim})")
    print(f"Mean execution time of {iters} runs: {mean}")

def matmulTest(dim):
    mat1 = Matrix(rows=dim, cols=dim, data=[x for x in range(dim*dim)])
    mat2 = Matrix(rows=dim, cols=dim, data=[x for x in range(dim*dim)])
    ts = time()
    mat2 @ mat1
    te = time()
    return te - ts
    

if __name__ == "__main__":
    timer(matmulTest)
    # pass
    # dim = 5
    # mat1 = Matrix(rows=dim, cols=dim, data=[x for x in range(dim*dim)])
    # print(mat1.T)
    # mat2 = Matrix(rows=dim, cols=dim, data=[x for x in range(dim*dim)])
    # display(mat1)
    # display(mat2)
    # add = mat1 + mat2
    # sub = mat1 - mat2
    # mul = mat1 * mat2
    # matmul = mat1 @ mat2
    # print("Addition:")
    # display(add)
    # print("Subtraction:")
    # display(sub)
    # print("Multiply:")
    # display(mul)
    # print("Matrix Multiply:")
    # display(matmul)

    # (mat2 @ mat1)

