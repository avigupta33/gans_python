'''
Layer class for neural networks
'''
from gans_types import *
from matrix import Matrix, Vector
from typing import *
import matrix_random as randm
import math

class Layer:

    def __init__(self) -> None:
        raise NotImplementedError


    def forwards(self, inputs: Vector) -> Vector:
        raise NotImplementedError


    def backwards(self, output_grads: Vector) -> Vector:
        raise NotImplementedError


    @staticmethod
    def vectorize(func: Mapping) -> VectorMapping:
        '''Only works on column vectors for speed purposes'''
        return (lambda vector, *argv:
            Vector(rows=vector.rows,
                   data=[func(x, *argv) for x in vector.unordered_data]
            )
        )


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


    def __init__(self, num_inputs: int, num_outputs: int, activator: str="ReLU") -> None:
        self.weights: Matrix = randm.gauss(rows=num_outputs, cols=num_inputs, mu=0., sigma=0.2)
        self.biases: Vector = Vector.zeros(rows=num_outputs)
        self.num_inputs = num_inputs
        self.num_outputs = num_outputs

        if activator not in (sub.__name__ for sub in FCLayer.Activator.__subclasses__()):
            raise ValueError(f"Activator '{activator}' doesn't exist")
        activator = getattr(FCLayer, activator)
        self.activate: VectorMapping = Layer.vectorize(activator.forwards)
        self.deactivate: VectorMapping = Layer.vectorize(activator.backwards)


    def forwards(self, inputs: Vector) -> Vector:
        if len(inputs) != self.layers[0].num_inputs:
            raise (
                ValueError(f"Trying to pass input with length {len(inputs)},"
                           f" but the this layer has {self.num_inputs}"
                           f"inputs/")
            )
        self.inputs: Vector = inputs
        self.post_mapping: Vector = self.weights @ self.inputs
        outputs: Vector = self.activate(self.post_mapping)
        return outputs

    def backwards(self, output_error: Vector) -> Vector:
        deactivated_output_error = self.deactivate(output_error)

        err_from_inputs = deactivated_output_error @ self.weights
        err_from_weights = deactivated_output_error.T @ self.inputs.T
        err_from_biases = deactivated_output_error

        return err_from_inputs, err_from_weights, err_from_biases

    def update(self, weight_update: Vector, bias_update: Vector) -> None:
        self.weights -= weight_update
        self.biases -= bias_update

    class ReLU(Activator):

        @staticmethod
        def forwards(input_scalar: T) -> T:
            return 0 if input_scalar <= 0 else input_scalar


        @staticmethod
        def backwards(output_error: T) -> T:
            return 0 if output_error <= 0 else 1


    class LeakyReLU(Activator):

        @staticmethod
        def forwards(input_scalar: T) -> T:
            return input_scalar*0.01 if input_scalar < 0 else input_scalar

        @staticmethod
        def backwards(output_error: T) -> T:
            return 0.01 if output_error <= 0 else 1


    class TanH(Activator):

        @staticmethod
        def forwards(input_scalar: T) -> T:
            return math.tanh(input_scalar)

        @staticmethod
        def backwards(output_error: T) -> T:
            return 1 - (math.tanh(output_error)**2)


# Example: 
if __name__ == "__main__":
    layer = FCLayer(10, 5, 'LeakyReLU')
    input_vector = randm.uniform(rows=10, lower_bound=-1, upper_bound=1)
    output_vector = layer.forwards(input_vector)
    output_vector.display()
