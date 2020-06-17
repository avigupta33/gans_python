from matrix import Matrix
from typing import *

class Layer:
    def __init__(self):
        pass

    def forwards(self, input):
        pass

    def backwards(self, input):
        pass

class FCLayer(Layer): #fully connected
    def __init__(self, inputs, outputs):
        self.weights = Matrix.random_gauss(rows = outputs, cols = inputs)
        self.biases = Matrix.zeros(rows = outputs, cols = 1)

    def forwards(self, input):
        return Matrix.multiply(input, self.weights)

