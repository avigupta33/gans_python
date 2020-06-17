from matrix import Matrix

if __name__ == "__main__":

    mat1 = Matrix.const(rows=4, cols=6, val=3)
    mat2 = Matrix.const(rows=6, cols=5, val=4)
    res = Matrix.multiply(mat1, mat2)

    mat1.display()
    mat2.display()
    res.display()

