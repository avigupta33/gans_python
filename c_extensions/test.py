from Matrix import *

mat1 = Matrix(cols=3, rows=1, data=[1,2,3])
mat2 = Matrix(cols=3, rows=1, data=[4,5,6])

print(mat1.getData())
print(mat2.getData())
print("Addition:", (mat1 + mat2).getData())
print("Subtraction:", (mat1 - mat2).getData())
print("Multiply:", (mat1 * mat2).getData())


mine  = (x for x in range(5))
mine