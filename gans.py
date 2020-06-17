from matrix import Matrix

if __name__ == "__main__":

    mat1 = Matrix.const(rows=4, cols=6, val=3)
    mat2 = Matrix.const(rows=4, cols=6, val=4)
    res = mat1 + mat2
    res.display()
