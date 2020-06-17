
'''
Layer class for neural networks
'''

from matrix import Matrix, T
from typing import *
import math

Vector = Matrix # 1D matrices only


class Layer:

    def __init__(self) -> None:
        raise NotImplementedError


    def forwards(self, input_vector: Vector) -> Vector:
        raise NotImplementedError


    def backwards(self, output_grad: Vector) -> Vector:
        raise NotImplementedError


class FCLayer(Layer):

    def __init__(self, num_inputs: int, num_outputs: int) -> None:
        self.weights: ClassVar[Matrix] = Matrix.random_gauss(rows=num_outputs, cols=num_inputs)
        self.biases: ClassVar[Vector] = Matrix.zeros(rows=num_outputs, cols=1)


    def forwards(self, input_vector: Vector) -> Vector:
        self.input_vector: ClassVar[Vector] = input_vector
        return Matrix.multiply(self.weights, input_vector)


    def backwards(self, output_grad: Vector) -> Vector:
        # need to discuss learning rate & stuff before implementing
        raise NotImplementedError


class ActivationLayer(Layer):

    def __init__(self, activator: str) -> None:
        if activator in ('forwards', 'backwards'):
            raise ValueError(f"Illegal activator name received: {activator}")
        if not hasattr(ActivationLayer, activator):
            raise ValueError(f"ActivationLayer has no {activator} activator")
        self.activator: ClassVar['Activator'] = getattr(ActivationLayer, activator)


    def forwards(self, input_vector: Vector) -> Vector:
        self.input_vector = input_vector
        data = [self.activator.forwards(x) for x in input_vector.data]
        return Matrix(input_vector.rows, input_vector.cols, data)


    def backwards(self, output_grad: Vector) -> Vector:
        raise NotImplementedError


    class Activator:
        # By storing everything explicitly in a class instead of 
        # a mutable data structure, python can optimize
        
        @staticmethod
        def forwards(input_scalar: T) -> T:
            raise NotImplementedError


        @staticmethod
        def backwards(output_grad: T) -> T:
            raise NotImplementedError


    class ReLU(Activator):

        @staticmethod
        def forwards(input_scalar: T) -> T:
            return max(input_scalar, 0)


        @staticmethod
        def backwards(output_grad: T) -> T:
            raise NotImplementedError


    class TanH(Activator):

        @staticmethod
        def forwards(input_scalar: T) -> T:
            return math.tanh(input_scalar)

        @staticmethod
        def backwards(output_grad: T) -> T:
            raise NotImplementedError


# Example: 
# x = ActivationLayer('ReLU')
# print(x.activator.forwards(-5))
# >>> 0
