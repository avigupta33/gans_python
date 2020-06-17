
'''
Layer class for neural networks
'''

from matrix import Matrix
from typing import *

Vector = Matrix # 1D matrices only

class Layer:
    def __init__(self) -> None:
        raise NotImplementedError

    def forwards(self, input_vector: Vector) -> Vector:
        raise NotImplementedError

    def backwards(self, output_grad: Vector) -> Vector:
        raise NotImplementedError

class FCLayer(Layer): # Fully connected
    def __init__(self, num_inputs: int, num_outputs: int) -> None:
        self.weights: ClassVar[Matrix] = Matrix.random_gauss(rows=num_outputs, cols=num_inputs)
        self.biases: ClassVar[Vector] = Matrix.zeros(rows=num_outputs, cols=1)

    def forwards(self, input_vector: Vector) -> Vector:
        self.input_vector: ClassVar[Vector] = input_vector
        return Matrix.multiply(self.weights, input_vector)

    def backwards(self, output_grad: Vector) -> Vector:
        # need to discuss learning rate & stuff before implementing
        raise NotImplementedError

