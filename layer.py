'''
Layer class for neural networks
'''
from gans_types import *
from matrix import Matrix
import matrix_random as matrand
import math

class Layer:

    def __init__(self) -> None:
        raise NotImplementedError


    def forwards(self, input_vector: Vector) -> Vector:
        raise NotImplementedError


    def backwards(self, output_grad: Vector) -> Vector:
        raise NotImplementedError


class FCLayer(Layer):

    class Activator:
        # By storing everything explicitly in a class instead of 
        # a mutable data structure, python can optimize
        
        @staticmethod
        def forwards(input_scalar: T) -> T:
            raise NotImplementedError


        @staticmethod
        def backwards(output_grad: T) -> T:
            raise NotImplementedError


        @staticmethod
        def vectorize(func: Mapping) -> VectorMapping:
            return (lambda vector, *argv: 
                Matrix(rows=vector.rows, cols=vector.cols, data=[
                    func(x, *argv) for x in vector.data
                ]))


    def __init__(self, num_inputs: int, num_outputs: int, activator: str="ReLU") -> None:
        self.weights: Matrix = matrand.gauss(rows=num_outputs, cols=num_inputs)
        self.biases: Vector = Matrix.zeros(rows=num_outputs, cols=1)

        if activator not in (sub.__name__ for sub in FCLayer.Activator.__subclasses__()):
            raise ValueError(f"Activator '{activator}' doesn't exist")
        activator = getattr(FCLayer, activator)
        self.activate: VectorMapping = self.Activator.vectorize(activator.forwards)
        self.deactivate: VectorMapping = self.Activator.vectorize(activator.backwards)


    def forwards(self, input_vector: Vector) -> Vector:
        self.input_vector: Vector = input_vector
        self.pre_activation: Vector= self.weights @ self.input_vector
        output_data: Vector = self.forwards_vectorized(self.pre_activation)
        return Matrix(self.weights.rows, 1, output_data)


    def backwards(self, output_grad: Vector) -> Vector:
        # need to discuss learning rate & stuff before implementing
        raise NotImplementedError


    class ReLU(Activator):

        @staticmethod
        def forwards(input_scalar: T) -> T:
            return 0 if input_scalar < 0 else input_scalar


        @staticmethod
        def backwards(output_grad: T) -> T:
            return 0 if output_grad <= 0 else 1


    class LeakyReLU(Activator):

        @staticmethod
        def forwards(input_scalar: T) -> T:
            return input_scalar*0.01 if input_scalar < 0 else input_scalar

        @staticmethod
        def backwards(output_grad: T) -> T:
            return 0.01 if output_grad <= 0 else 1


    class TanH(Activator):

        @staticmethod
        def forwards(input_scalar: T) -> T:
            return math.tanh(input_scalar)

        @staticmethod
        def backwards(output_grad: T) -> T:
            return 1 - (math.tanh(output_grad)**2)


# Example: 
# x = FCLayer(100, 5, 'LeakyReLU')
# x.activate(Matrix(1,1,[-100])).display()
# >>> 0
