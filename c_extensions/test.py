from Quantum import Matrix

mat1 = Matrix(cols=3, rows=4, data=[5 for _ in range(12)])
mat2 = Matrix(cols=3, rows=1, data=[4,5,6])

def display(mat):
    r, c, data = mat.rows, mat.cols, mat.data
    index = 0
    for row_i in range(r):
        print('\t'.join(str(data[row_i * c + col_i]) for col_i in range(c)).expandtabs(3))

display(mat1)

# print(mat1.data)
# print(mat2.data)
# print("Addition:", (mat1 + mat2).data)
# print("Subtraction:", (mat1 - mat2).data)
# print("Multiply:", (mat1 * mat2).data)

