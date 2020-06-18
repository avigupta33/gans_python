import math
from matrix import Vector

def binary_cross_entropy_loss(labels: Vector, preds: Vector) -> float:
    return -1/len(preds) * sum(y * math.log(p) + (1-y) * math.log(1-p)
                               for y,p in zip(labels.data, preds.data))
