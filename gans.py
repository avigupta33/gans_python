from matrix import Matrix

if __name__ == "__main__":

    mat1 = Matrix.const(4,6,3)
    mat2 = Matrix.const(6,5,4)
    res = Matrix.multiply(mat1, mat2)
    res.display()
