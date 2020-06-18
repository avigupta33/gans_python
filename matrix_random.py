'''
Matrix random generator methods
'''
from gans_types import *
import random

def uniform(rows: int, cols: int, lower_bound: float, upper_bound: float) -> Matrix:
    return Matrix.generate(rows, cols, lambda: random.uniform(lower_bound, upper_bound))


def gauss(rows: int, cols: int, mu: float=0., sigma: float=0.2) -> Matrix:
    return Matrix.generate(rows, cols, lambda: random.gauss(mu, sigma))


def sample(mat: Vector, num_samples: int) -> Vector:
    if num_samples > len(self):
        raise ValueError(f"Requested {num_samples} samples, "
                         f"only {len(self)} available")
    return Matrix(rows=num_samples, cols=1, data=random.sample(self._data, num_samples))


def noise_vector(length: int, mu: float=0., sigma: float=1.) -> Vector:
    # Matrix() raises exception is rows <= 0 so this doesn't have to
    return Matrix.generate(rows=length, cols=1, generator=lambda: random.gauss(mu, sigma))

