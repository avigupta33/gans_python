from matrix import Matrix
from typing import *
import math
"""
 D. P. Kingma and J. Ba, "Adam: A Method for Stochastic Optimization," 
 CoRR, vol. abs/1412.6980, 2014. [Online]. Available: A Method for Stochastic Optimization
"""
class AdamOptimizer:
    def __init__(self, params, alpha = 0.001,
                 b1 = 0.9, b2 = 0.99, eps = 10 ** (-8)):
        self.params = params
        self.alpha = alpha
        self.b1 = b1
        self.b2 = b2
        self.eps = eps
        self.m = 0
        self.v = 0
        self.t = 0

    def back_pass(self, grad):
        self.t+=1
        self.m = self.b1 * self.m + (1 - self.b1) * grad
        self.v = self.b2 * self.m + (1 - self.b2) * (grad ** 2)
        m_hat = self.m/(1-self.b1)
        v_hat = self.v/(1-self.b2)
        self.params -= self.alpha * m_hat/(math.sqrt(v_hat) - self.eps)
        return self.params










