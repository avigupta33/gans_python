from matrix import Matrix
from loss_functions import binary_cross_entropy_loss

if __name__ == "__main__":
    print("Hello, World!")
    test = Matrix(97,1, [x/100 for x in range(1,98)])
    test2 = Matrix(97,1,[x/100 for x in range(2,99)])
    print(binary_cross_entropy_loss(test, test2))
