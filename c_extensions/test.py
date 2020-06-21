from Quantum import Matrix

def display(mat):
    r, c, data = mat.rows, mat.cols, mat.data
    index = 0
    for row_i in range(r):
        print('\t'.join(str(data[row_i * c + col_i]) for col_i in range(c)).expandtabs(3))

from time import time

def timer(func, iters=1000):
    dim = 100
    mean = sum(func(dim) for _ in range(iters))/iters
    print(f"C ({dim},{dim}) @ ({dim},{dim})")
    print(f"Mean execution time of {iters} runs: {mean}")

def matmulTest(dim):
    mat1 = Matrix(rows=dim, cols=dim, data=[x for x in range(10000)])
    mat2 = Matrix(rows=dim, cols=dim, data=[x for x in range(10000)])
    ts = time()
    mat2 @ mat1
    te = time()
    return te - ts
    

if __name__ == "__main__":
    timer(matmulTest)
    # pass
    # print(mat1.data)
    # print(mat2.data)
    # print("Addition:", (mat1 + mat2).data)
    # print("Subtraction:", (mat1 - mat2).data)
    # print("Multiply:", (mat1 * mat2).data)
    # print(mat1.rows)
    # print(mat1.cols)
    # print(mat1)

    # (mat2 @ mat1)

