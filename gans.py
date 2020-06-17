from matrix import Matrix

if __name__ == "__main__":

    array = [
        0,1,2,
        3,4,5,
        6,7,8
    ]
    mat = Matrix(3,3,array)

    mat1 = Matrix.const(rows=4, cols=6, val=3)
    mat2 = Matrix.const(rows=6, cols=5, val=4)
    res = Matrix.multiply(mat1, mat2)



    # mat1.display()
    # mat2.display()
    # print(res.data)
    res.display()

    # def factory():
    #     return (x for x in range(5))

    # for i in range(3):
    #     for num in factory():
    #         print(num)
