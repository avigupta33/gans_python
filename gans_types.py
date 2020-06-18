'''
Typing variables for project scope
'''
from typing import *
from matrix import Matrix, T

# 1D matrix
Vector: Type = Matrix

# Activation functions
Mapping: Type = Callable[[T], T] 

# Vectorized activation functions
VectorMapping: Type = Callable[[Vector], Vector]  

