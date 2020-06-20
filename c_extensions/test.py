from Quantum import Matrix


def display(mat):
    r, c, data = mat.rows, mat.cols, mat.data
    index = 0
    for row_i in range(r):
        print('\t'.join(str(data[row_i * c + col_i]) for col_i in range(c)).expandtabs(3))


if __name__ == "__main__":
    # pass
    mat1 = Matrix(cols=3, rows=1, data=[1,2,3])
    mat2 = Matrix(cols=3, rows=1, data=[4,5,6])

    print(mat1.data)
    print(mat2.data)
    print("Addition:", (mat1 + mat2).data)
    print("Subtraction:", (mat1 - mat2).data)
    print("Multiply:", (mat1 * mat2).data)
    print(mat1.rows)
    print(mat1.cols)
    print(mat1)

