from typing import *
import random

class Matrix:
    def __init__(self, data: List[List[int]]) -> None:
        self.data = data
        self.rows = len(data)
        self.cols = len(data[0])

    def __repr__(self):
        return f"Matrix with {self.rows} rows and {self.cols} cols"

    def __getitem__(self, key) -> int:
        return getattr(self.data, key)

    def __setitem__(self, key) -> int:
        return setattr(self.data, key)

    def display(self, tabspace=3) -> None:
        print('\n'.join('\t'.join(str(v) for v in row).expandtabs(tabspace) for row in self.data))

    def getRow(self, row_i) -> List[int]:
        if row_i >= self.rows or row_i <0:
            raise ValueError(f"Matrix with {self.rows} does not have row at {row_i}")
        return self.data[row_i]

    def getCol(self, col_i) -> List[int]:
        if col_i >= self.cols or col_i <0:
            raise ValueError(f"Matrix with {self.cols} does not have col at {col_i}")
        return [row[col_i] for row in self.data]

    @classmethod
    def zeros(cls, rows, cols):
        return cls([[0 for _ in range(cols)] for _ in range(rows)])

    @classmethod
    def const(cls, rows, cols, val):
        return cls([[val for _ in range(cols)] for _ in range(rows)])

    @classmethod
    def random(cls, rows, cols, lower_bound, upper_bound):
        return cls([[random.uniform(lower_bound, upper_bound) for _ in range(cols)] for _ in range(rows)])

    @staticmethod
    def multiply(mat1, mat2):
        if mat1.cols != mat2.rows:
            raise ValueError(f"Matrix 1 has dims {mat1.rows, mat1.cols} "
                             f"while Matrix 2 has dims {mat2.rows, mat2.cols}. "
                             f"Incompatible for multiplication")
        result = [[0 for _ in range(mat2.cols)] for _ in range(mat1.rows)]
        for row_i in range(mat1.rows):
            for col_i in range(mat2.cols):
                dp = sum([a*b for a,b in zip(mat1.getRow(row_i), mat2.getCol(col_i))])

                result[row_i][col_i] = dp

        return Matrix(result)
