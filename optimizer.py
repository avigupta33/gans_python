from matrix import Matrix
import typing
from typing import *
Vector = Matrix # 1D matrices only

import math
"""
 D. P. Kingma and J. Ba, "Adam: A Method for Stochastic Optimization," 
 CoRR, vol. abs/1412.6980, 2014. [Online]. Available: A Method for Stochastic Optimization
"""
class AdamOptimizer:
    def __init__(self, params: Matrix, alpha: float= 0.001,
                 b1:float = 0.9, b2: float = 0.99, eps: float = 1E-8) -> None:
        self.params = params
        self.alpha = alpha
        self.b1 = b1
        self.b2 = b2
        self.eps = eps
        self.m = 0
        self.v = 0
        self.t = 0

    def back_pass(self, grad: Vector ) -> Matrix:
        self.t+=1
        self.m = self.b1 * self.m + (1 - self.b1) * grad
        self.v = self.b2 * self.m + (1 - self.b2) * (grad ** 2) #TODO make grad squared
        m_hat = self.m/(1-self.b1)
        v_hat = self.v/(1-self.b2)
        self.params -= self.alpha * m_hat/(math.sqrt(v_hat) + self.eps)
        return self.params










