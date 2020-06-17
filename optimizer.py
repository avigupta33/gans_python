from matrix import Matrix
from typing import *
"""
 D. P. Kingma and J. Ba, "Adam: A Method for Stochastic Optimization," 
 CoRR, vol. abs/1412.6980, 2014. [Online]. Available: A Method for Stochastic Optimization
"""
class AdamOptimizer:
    def __init__(self, step_size = 0.001,
                 b1 = 0.9, b2 = 0.99, delta = 10 ** (-8),
                 minibatch_size = 16):
        self.step_size = step_size
        self.p1 = p1
        self.p2 = p2
        self.delta = delta

    def optimize(self, params, training_set):
        while True: #TODO: Figure out stopping condition


