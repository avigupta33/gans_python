from matrix import Matrix
from typing import *

class Layer:
    def __init__(self):
        pass

    def forwards(self):
        pass

    def backwards(self):
        pass

class FCLayer(Layer): #fully connected
    def __init__(self, weights):
        self.weights = weights