from Matrix import *

mat1 = Matrix(cols=3, rows=1, data=[1,2,3])
mat2 = Matrix(cols=3, rows=1, data=[4,5,6])

print(mat1.data)
print(mat2.data)
print("Addition:", (mat1 + mat2).data)
print("Subtraction:", (mat1 - mat2).data)
print("Multiply:", (mat1 * mat2).data)

