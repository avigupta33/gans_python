
'''
Matrix class for neural networks
'''

from typing import *
import random

T = Any # Matrix type
VectorIter = Iterator[T]

class Matrix:

    def __init__(self, rows: int, cols: int, data: List[T]) -> None:
        if (rows <= 0):
            raise ValueError(f"Rows must be positive, is {rows}")
        if (cols <= 0):
            raise ValueError(f"Cols must be positive, is {cols}")
        self.rows: ClassVar[int] = rows
        self.cols: ClassVar[int] = cols
        self.data: ClassVar[List[T]] = data

    def __len__(self):
        if self.rows != 1 and self.cols != 1:
            raise ValueError(f"Len is supported only for row/col. vectors. "
                             f"Matrix has dims {self.rows, self.cols}.")
        else:
            return min(self.rows, self.cols)

    def __repr__(self) -> str:
        return f"Matrix with {self.rows} rows and {self.cols} cols"


    def __getitem__(self, key: Tuple[int, int]) -> T:
        row, col = key
        return self.data[row * self.cols + col]


    def __setitem__(self, key: Tuple[int, int], value: T) -> None:
        row, col = key
        self.data[row * self.cols + col] = value


    def __add__(self, m: 'Matrix') -> 'Matrix':
        if self.cols != m.cols or self.rows != m.rows:
            raise ValueError(f"Matrix A has dims {self.rows, self.cols} "
                             f"while Matrix B has dims {m.rows, m.cols}. "
                             f"Incompatible for addition")

        data = [a+b for a,b in zip(self.data, m.data)]
        return Matrix(self.rows, self.cols, data)

    def __sub__(self, m: 'Matrix') -> 'Matrix':
        if self.cols != m.cols or self.rows != m.rows:
            raise ValueError(f"Matrix A has dims {self.rows, self.cols} "
                             f"while Matrix B has dims {m.rows, m.cols}. "
                             f"Incompatible for addition")

        data = [a-b for a,b in zip(self.data, m.data)]
        return Matrix(self.rows, self.cols, data)


    def __eq__(self, m: 'Matrix') -> bool:
        return (self.rows == m.rows and
                self.cols == m.cols and
                not any(a != b for a,b in zip(self.data, m.data)))


    def __matmul__(self, m: 'Matrix') -> 'Matrix':
        if self.cols != m.rows:
            raise ValueError(f"Matrix A has dims {self.rows, self.cols} "
                             f"while Matrix B has dims {m.rows, m.cols}. "
                             f"Incompatible for multiplication")

        data = [sum(a*b for a,b in zip(row(), col())) for row in self.iterRow() for col in m.iterCol()]

        return Matrix(self.rows, m.cols, data)


    @property
    def t(self):
        #TODO add transposition
        pass


    def display(self, tabspace=3) -> None:
        print('\n'.join('\t'.join(str(x) for x in row()).expandtabs(tabspace) for row in self.iterRow()))


    def getRow(self, row_i: int) -> VectorIter:
        if row_i >= self.rows or row_i < 0:
            raise ValueError(f"Matrix with {self.rows} does not have row at {row_i}")
        return (self.data[x] for x in range(row_i * self.cols, (row_i + 1) * self.cols))


    def getCol(self, col_i: int) -> VectorIter:
        if col_i >= self.cols or col_i <0:
            raise ValueError(f"Matrix with {self.cols} does not have col at {col_i}")
        return (self.data[x] for x in range(col_i, self.rows * self.cols, self.cols))


    def iterRow(self) -> Generator[Callable[[], VectorIter], None, None]:
        '''Generates VectorIter factory objects for rows'''
        for row_begin in range(0, self.rows*self.cols, self.cols):
            yield lambda: (self.data[x] for x in range(row_begin, row_begin + self.cols))


    def iterCol(self) -> Generator[Callable[[], VectorIter], None, None]:
        '''Generates VectorIter factory objects for cols'''
        for col_i in range(self.cols):
            yield lambda: (self.data[x] for x in range(col_i, self.rows*self.cols, self.cols))


    @classmethod
    def zeros(cls, rows: int, cols: int) -> 'Matrix':
        return cls.const(rows, cols, 0)


    @classmethod
    def const(cls, rows: int, cols: int, val: T) -> 'Matrix':
        return cls(rows, cols, [val for _ in range(rows * cols)])


    @classmethod
    def random_uni(cls, rows: int, cols: int, lower_bound, upper_bound) -> 'Matrix':
        return cls(rows, cols, [random.uniform(lower_bound, upper_bound) for _ in range(rows * cols)])


    @classmethod
    def random_gauss(cls, rows: int, cols: int) -> 'Matrix':
        return cls(rows, cols, [random.gauss(mu=0, sigma=0.2) for _ in range(rows * cols)])
        # These values are ideal for GANs apparently


    @classmethod
    def noise_vector(cls, length: int) -> 'Vector':
        return cls(length, 1, [random.gauss(mu = 0, sigma=1)])





