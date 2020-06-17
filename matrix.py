from typing import *
import random

class Matrix:

    def __init__(self, rows: int, cols: int, data: List[int]) -> None:
        if (rows <= 0):
            raise ValueError("Rows must be positive, is {rows}")
        if (cols <= 0):
            raise ValueError("Cols must be positive, is {cols}")
        self.rows = rows
        self.cols = cols
        self.data = data


    def __repr__(self):
        return f"Matrix with {self.rows} rows and {self.cols} cols"

    def __getitem__(self, key) -> int:
        row, col = key
        return self.data[row * self.cols + col]

    def __setitem__(self, key, value) -> int:
        row, col = key
        self.data[row * self.cols + col] = value

    def display(self, tabspace=3) -> None:
        # might be able to optimize more now that data is flattened
        print('\n'.join('\t'.join(str(v) for v in self.getRow(row_i)).expandtabs(tabspace) for row_i in range(self.rows)))

    def getRow(self, row_i) -> Sequence[int]:
        if row_i >= self.rows or row_i < 0:
            raise ValueError(f"Matrix with {self.rows} does not have row at {row_i}")
        return (self.data[x] for x in range(row_i * self.cols, (row_i + 1) * self.cols))

    def getCol(self, col_i) -> Sequence[int]:
        if col_i >= self.cols or col_i <0:
            raise ValueError(f"Matrix with {self.cols} does not have col at {col_i}")
        return (self.data[x] for x in range(col_i, self.rows * self.cols, self.cols))

    @classmethod
    def zeros(cls, rows, cols):
        return cls.const(rows, cols, 0)

    @classmethod
    def const(cls, rows, cols, val):
        return cls(rows, cols, [val for _ in range(rows * cols)])

    @classmethod
    def random(cls, rows, cols, lower_bound, upper_bound):
        return cls([random.uniform(lower_bound, upper_bound) for _ in range(rows * cols)])

    @staticmethod
    def multiply(mat1, mat2):
        if mat1.cols != mat2.rows:
            raise ValueError(f"Matrix 1 has dims {mat1.rows, mat1.cols} "
                             f"while Matrix 2 has dims {mat2.rows, mat2.cols}. "
                             f"Incompatible for multiplication")

        def rowFactory(mat):
            begin, end = 0, mat.cols
            for row_i in range(mat.rows):
                yield lambda: (mat.data[x] for x in range(begin, end))
                begin, end = end, end + mat.cols

        def colFactory(mat):
            for col_i in range(mat.cols):
                yield lambda: (mat.data[x] for x in range(col_i, mat.rows * mat.cols, mat.cols))
        
        array = []
        for col in colFactory(mat2):
            for row in rowFactory(mat1):
                # Here, getRow() and getCol() act as generator factories by returning fresh generators
                # How can we make it so that we don't need to pass them indices?
                # Make a function that returns a generator that returns a function that returns a generator that returns an int :)
                # Basically, make a iterCols() funcion that will give you a generator that 

                array.append(sum(a*b for a,b in zip(row(), col())))

        return Matrix(mat1.rows, mat2.cols, array)
