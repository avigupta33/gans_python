from Quantum import Matrix


def display(mat):
    r, c, data = mat.rows, mat.cols, mat.data
    index = 0
    for row_i in range(r):
        print('\t'.join(str(data[row_i * c + col_i]) for col_i in range(c)).expandtabs(3))


if __name__ == "__main__":
    # pass
    mat1 = Matrix(rows=4, cols=3, data=[x for x in range(12)])
    mat2 = Matrix(rows=1, cols=4, data=[x for x in range(4)])

    # print(mat1.data)
    # print(mat2.data)
    # print("Addition:", (mat1 + mat2).data)
    # print("Subtraction:", (mat1 - mat2).data)
    # print("Multiply:", (mat1 * mat2).data)
    # print(mat1.rows)
    # print(mat1.cols)
    # print(mat1)
    display(mat2 @ mat1)

