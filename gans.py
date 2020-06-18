from matrix import *
from matrix_random import *

if __name__ == "__main__":
    print("Hello, World!")
    # data = [
    #     1,2,3,
    #     4,5,6,
    #     7,8,9
    # ]
    # mat = Matrix(3,3,data)

    uniform(rows=5, cols=5, lower_bound=0, upper_bound=4).display()
    print()
    gauss(rows=5, cols=5).display()




