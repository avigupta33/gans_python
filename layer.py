'''
Layer class for neural networks
'''
from Quantum import Matrix
from typing import *
import math

Vector = Matrix

class Layer:

    def __init__(self) -> None:
        raise NotImplementedError


    def forwards(self, inputs: Vector) -> Vector:
        raise NotImplementedError


    def backwards(self, output_grads: Vector) -> Vector:
        raise NotImplementedError

class FCLayer(Layer):

    class Activator:
        # By storing everything explicitly in a class instead of 
        # a mutable data structure, python can optimize
        
        @staticmethod
        def forwards(input_scalar: float) -> float:
            raise NotImplementedError


        @staticmethod
        def backwards(output_grad: float) -> float:
            raise NotImplementedError


    def __init__(self, num_inputs: int, num_outputs: int, activator: str="ReLU") -> None:
        self.weights: Matrix = Matrix.gauss(dims = (num_outputs, num_inputs), mu=0., sigma=0.2)
        self.biases: Vector = Vector(0, dims=(num_outputs, ))
        self.num_inputs = num_inputs
        self.num_outputs = num_outputs

        if activator not in (sub.__name__ for sub in FCLayer.Activator.__subclasses__()):
            raise ValueError(f"Activator '{activator}' doesn't exist")
        self.activator = getattr(FCLayer, activator)


    def forwards(self, inputs: Vector) -> Vector:
        if inputs.rows != self.num_inputs:
            raise (
                ValueError(f"Trying to pass input with {inputs.rows} rows"
                           f" but the this layer has {self.num_inputs}"
                           f"inputs/")
            )
        self.inputs: Vector = inputs
        self.post_mapping: Vector = self.weights @ self.inputs
        outputs: Vector = self.post_mapping.map(self.activator.forwards)
        return outputs


    def backwards(self, output_error: Vector) -> Vector:
        deactivated_output_error = output_error.map(self.activator.backwards)

        err_from_inputs = deactivated_output_error @ self.weights
        err_from_weights = deactivated_output_error.T @ self.inputs.T
        err_from_biases = deactivated_output_error

        return err_from_inputs, err_from_weights, err_from_biases


    def update(self, weight_update: Vector, bias_update: Vector) -> None:
        self.weights -= weight_update
        self.biases -= bias_update


    class ReLU(Activator):

        @staticmethod
        def forwards(input_scalar: float) -> float:
            return 0 if input_scalar <= 0 else input_scalar


        @staticmethod
        def backwards(output_error: float) -> float:
            return 0 if output_error <= 0 else 1


    class LeakyReLU(Activator):

        @staticmethod
        def forwards(input_scalar: float) -> float:
            return input_scalar*0.01 if input_scalar < 0 else input_scalar

        @staticmethod
        def backwards(output_error: float) -> float:
            return 0.01 if output_error <= 0 else 1


    class TanH(Activator):

        @staticmethod
        def forwards(input_scalar: float) -> float:
            return math.tanh(input_scalar)

        @staticmethod
        def backwards(output_error: float) -> float:
            return 1 - (math.tanh(output_error)**2)


# Example: 
if __name__ == "__main__":
    layer = FCLayer(10, 5, 'LeakyReLU')
    input_vector = Matrix.uniform(dims=(10,), lower=-1, upper=1)
    output_vector = layer.forwards(input_vector)
    print(output_vector)
