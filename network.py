from layer import Layer
from typing import *
from matrix import Matrix
Vector = Matrix

class Network:
    def __init__(self, layers: List[Layer]) -> None:
        self.layers = layers

    def addLayer(self, l: Layer) -> None:
        if l.num_inputs != self.layers[-1].num_outputs:
            raise(
                ValueError(f"Trying to add a layer with {l.num_inputs} inputs,"
                           f" but the last layer currently has {self.layers[-1].num_outputs}"
                           f"outputs/")
            )

        self.layers.append(l)

    def forwards(self, inputs: Vector) -> Vector:
        outputs = self.layers[0].forwards(inputs)
        for i in range(1, len(self.layers)):
            outputs = self.layers[i].forwards(outputs)
        return outputs

    def backwards(self, cost: Union[float, int]) -> Vector:









