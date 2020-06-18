from matrix import Matrix

if __name__ == "__main__":
    print("Hello, World!")
    data = [
        1,2,3,
        4,5,6,
        7,8,9
    ]
    mat = Matrix(3,3,data)
    mat.t.display()

    (mat.sample(10)).display()

