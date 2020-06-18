"""
 D. P. Kingma and J. Ba, "Adam: A Method for Stochastic Optimization," 
 CoRR, vol. abs/1412.6980, 2014. [Online]. Available: A Method for Stochastic Optimization
"""
from typing import *
import typing
import math
from matrix import Matrix


class AdamOptimizer:

    def __init__(self, params: Matrix, alpha: float=1E-3,
                 b1:float=0.9, b2: float=0.99, eps: float=1E-8) -> None:
        # can you attempt to add descriptions for these variable names?
        self.params: Matrix = params
        self.alpha: float = alpha
        self.b1: float = b1
        self.b2: float = b2
        self.eps: float = eps
        self.m = 0
        self.v = 0
        self.t = 0


    def step(self, grad: Vector ) -> Matrix:
        self.t+=1
        self.m = self.b1 * self.m + (1 - self.b1) * grad
        self.v = self.b2 * self.m + (1 - self.b2) * (grad * grad)
        m_hat = self.m/(1-self.b1)
        v_hat = self.v/(1-self.b2)
        self.params -= self.alpha * m_hat/(math.sqrt(v_hat) + self.eps)
        return self.params










