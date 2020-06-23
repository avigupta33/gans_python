import Quantum as q

def display(mat, tabspace=3):
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
    mat1 = q.Matrix(rows=dim, cols=dim, data=[x for x in range(dim*dim)])
    mat2 = q.Matrix(rows=dim, cols=dim, data=[x for x in range(dim*dim)])
    ts = time()
    mat2 @ mat1
    te = time()
    return te - ts
    

if __name__ == "__main__":
    # timer(matmulTest)
    # pass
    # dim = 5
    # mat1 = Matrix(rows=dim, cols=dim, data=[x for x in range(1,1+dim*dim)])
    # mat2 = q.Matrix(dim, dim, [x for x in range(1,1+dim*dim)])
    # display(mat1/10)
    # zeros = q.zeros(cols=1, rows=4)
    # display(zeros)
    # print()
    # display(mat1)
    # fives = q.fill(4, 1, 5)
    # display(fives)
    uniform = q.uniform(rows=10,cols=1,lower_bound=-1,upper_bound=9)
    display(uniform)
    # print(mat1* 8)
    # display(mat1*2)
    # display(mat1*1.5)
    # display(2*mat1)
    # display(1.5*mat1)

    # add = mat1 + mat2
    # sub = mat1 - mat2
    # mul = mat1 * mat2
    # matmul = mat1 @ mat2
    # print("Addition:")
    # display(add)
    # print()
    # print("Subtraction:")
    # display(sub)
    # print()
    # print("Multiply:")
    # display(mul)
    # print()
    # print("Matrix Multiply:")
    # display(matmul)
    # print()

    # (mat2 @ mat1)

