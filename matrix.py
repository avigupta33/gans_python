from typing import *
import random

T = Any # matrix type
VectorGenerator = Generator[T, None, None]

class Matrix:

    def __init__(self, rows: int, cols: int, data: List[T]) -> None:
        if (rows <= 0):
            raise ValueError("Rows must be positive, is {rows}")
        if (cols <= 0):
            raise ValueError("Cols must be positive, is {cols}")
        self.rows: ClassVar[int] = rows
        self.cols: ClassVar[int] = cols
        self.data: ClassVar[List[T]] = data


    def __repr__(self) -> str:
        return f"Matrix with {self.rows} rows and {self.cols} cols"


    def __getitem__(self, key: Tuple[int, int]) -> T:
        row, col = key
        return self.data[row * self.cols + col]


    def __setitem__(self, key: Tuple[int, int], value: T) -> None:
        row, col = key
        self.data[row * self.cols + col] = value


    def display(self, tabspace=3) -> None:
        print('\n'.join('\t'.join(str(x) for x in row()).expandtabs(tabspace) for row in self.iterRow()))


    def getRow(self, row_i: int) -> VectorGenerator:
        if row_i >= self.rows or row_i < 0:
            raise ValueError(f"Matrix with {self.rows} does not have row at {row_i}")
        return (self.data[x] for x in range(row_i * self.cols, (row_i + 1) * self.cols))


    def getCol(self, col_i: int) -> VectorGenerator:
        if col_i >= self.cols or col_i <0:
            raise ValueError(f"Matrix with {self.cols} does not have col at {col_i}")
        return (self.data[x] for x in range(col_i, self.rows * self.cols, self.cols))


    def iterRow(self) -> Iterator[Callable[[], VectorGenerator]]:
        for row_i in range(self.rows):
            yield lambda: (self.data[x] for x in range(row_i * self.cols, (row_i + 1) * self.cols))


    def iterCol(self) -> Iterator[Callable[[], VectorGenerator]]:
        for col_i in range(self.cols):
            yield lambda: (self.data[x] for x in range(col_i, self.rows*self.cols, self.cols))


    @classmethod
    def zeros(cls, rows: int, cols: int) -> 'Matrix':
        return cls.const(rows, cols, 0)


    @classmethod
    def const(cls, rows: int, cols: int, val: T) -> 'Matrix':
        return cls(rows, cols, [val for _ in range(rows * cols)])


    @classmethod
    def random(cls, rows: int, cols: int, lower_bound, upper_bound) -> 'Matrix':
        return cls([random.uniform(lower_bound, upper_bound) for _ in range(rows * cols)])


    @staticmethod
    def multiply(first: 'Matrix', second: 'Matrix') -> 'Matrix':
        if first.cols != second.rows:
            raise ValueError(f"Matrix A has dims {first.rows, first.cols} "
                             f"while Matrix B has dims {second.rows, second.cols}. "
                             f"Incompatible for multiplication")

        array = [sum(a*b for a,b in zip(row(), col())) for row in first.iterRow() for col in second.iterCol()]

        return Matrix(first.rows, second.cols, array)
