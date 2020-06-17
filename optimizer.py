from matrix import Matrix
from typing import *
"""
 D. P. Kingma and J. Ba, "Adam: A Method for Stochastic Optimization," 
 CoRR, vol. abs/1412.6980, 2014. [Online]. Available: A Method for Stochastic Optimization
"""
class AdamOptimizer:
    def __init__(self, step_size = 0.001,
                 b1 = 0.9, b2 = 0.99, epsilon = 10 ** (-8),
                 minibatch_size = 16):
        self.step_size = step_size
        self.b1 = b1
        self.b2 = b2
        self.epsilon = epsilon

    def optimize(self, params, training_set, objective_func):
        m0 = 0
        v0 = 0
        t = 0
        while True: #TODO: Figure out stopping condition
            t+=1






