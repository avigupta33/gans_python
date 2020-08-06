import math
from Quantum import Matrix
Vector = Matrix

def binary_cross_entropy_loss(labels: Vector, preds: Vector) -> float:
    return -1/preds.rows * sum(y * math.log(p) + (1-y) * math.log(1-p)
                               for y,p in zip(labels.data, preds.data))

# Mean Squared Error
def mse(labels: Vector, preds: Vector) -> float:
    return 0.5 * (labels - preds).map(lambda x: x**2)

if __name__ == "__main__":
    x = Matrix([1,1,1,1,1], dims = (5, 1))
    y = Matrix([0,0,0,0,0], dims = (5, 1))
    print(mse(x,y))