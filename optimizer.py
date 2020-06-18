"""
 D. P. Kingma and J. Ba, "Adam: A Method for Stochastic Optimization," 
 CoRR, vol. abs/1412.6980, 2014. [Online]. Available: A Method for Stochastic Optimization
"""
from typing import *
import typing
import math
from matrix import Matrix

Vector = Matrix

class AdamOptimizer:

    def __init__(self, params: Matrix, alpha: float=1E-3,
                 b1:float=0.9, b2: float=0.99, eps: float=1E-8) -> None:

        self.params: Matrix = params #params to optimize
        self.alpha: float = alpha #Step size
        self.b1: float = b1 #Exponential decay rate for first moment
        self.b2: float = b2 #Exponential decay rate for second moment
        self.eps: float = eps #small number to prevent division by 0
        self.m = 0 #Initialize first moment vector
        self.v = 0 #Initialize second moment vector
        self.t = 0 #timestep


    def step(self, grad: Vector ) -> Matrix:
        self.t+=1
        self.m = self.b1 * self.m + (1 - self.b1) * grad
        self.v = self.b2 * self.m + (1 - self.b2) * (grad * grad)
        m_hat = self.m/(1-self.b1) #Compute bias-corrected 1st moment estimate
        v_hat = self.v/(1-self.b2) #Compute bias-corrected 2nd moment estimate
        self.params -= self.alpha * m_hat/(math.sqrt(v_hat) + self.eps)
        return self.params










