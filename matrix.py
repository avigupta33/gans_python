'''
Matrix class for neural networks
'''
from typing import TypeVar, List, Iterator, Iterable, Callable

# Matrix type
T = TypeVar('T')

class Matrix:

    def __init__(self, rows: int, cols: int, data: List[T]) -> None:

        # Assert rows and cols are valid
        if rows <= 0:
            raise ValueError(f"Rows must be positive, is {rows}")
        if cols <= 0:
            raise ValueError(f"Cols must be positive, is {cols}")

        self.rows: int = rows
        self.cols: int = cols

        # Assert received correct number of elements
        if len(data) != len(self):
            raise ValueError(f"Expected {len(self)} elements, but "
                             f"received {len(data)} elements")
        self._data: List[T] = data

        def iterRow() -> Iterator[Callable[[], Iterable[T]]]:
            for row_begin in range(0, self.rows*self.cols, self.cols):
                yield lambda: (self._data[x] for x in range(row_begin, row_begin + self.cols))

        def iterCol() -> Iterator[Callable[[], Iterable[T]]]:
            for col_i in range(self.cols):
                yield lambda: (self._data[x] for x in range(col_i, self.rows*self.cols, self.cols))

        self.iterRow = iterRow
        self.iterCol = iterCol


    def __repr__(self) -> str:
        return f"Matrix with {self.rows} rows and {self.cols} cols"


    def __len__(self) -> int:
        return self.rows * self.cols


    def __add__(self, m: 'Matrix') -> 'Matrix':
        return Matrix.compress(self, m, lambda a,b: a+b)


    def __sub__(self, m: 'Matrix') -> 'Matrix':
        return Matrix.compress(self, m, lambda a,b: a-b)


    def __mul__(self, m: 'Matrix') -> 'Matrix':
        return Matrix.compress(self, m, lambda a,b: a*b)


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
    def data(self) -> Iterable[T]:
        return (x for row in self.iterRow() for x in row())


    @property
    def t(self) -> 'Matrix':
        trans = Matrix(rows=self.cols, cols=self.rows, data=self._data)
        trans.iterRow, trans.iterCol = self.iterCol, self.iterRow
        return trans


    def display(self, tabspace: int=3) -> None:
        print('\n'.join('\t'.join(str(x) for x in row()).expandtabs(tabspace) for row in self.iterRow()))


    @classmethod
    def zeros(cls, rows: int, cols: int) -> 'Matrix':
        return cls.const(rows, cols, 0)


    @classmethod
    def const(cls, rows: int, cols: int, val: T) -> 'Matrix':
        return cls(rows, cols, [val for _ in range(rows * cols)])


    @classmethod
    def generate(cls, rows: int, cols: int, generator: Callable[[], T]) -> 'Matrix':
        return cls(rows, cols, [generator() for _ in range(rows * cols)])

    
    @classmethod
    def compress(cls, mat1: 'Matrix', mat2: 'Matrix', combinator: Callable[[T, T], T]) -> 'Matrix':
        if mat1.cols != mat2.cols or mat1.rows != mat2.rows:
            raise ValueError(f"Matrix 1 has dims {mat1.rows, mat1.cols} "
                             f"while Matrix 2 has dims {mat2.rows, mat2.cols}. "
                             f"Incompatible for folding")

        data = [combinator(a,b) for a,b in zip(mat1.data, mat2.data)]
        return Matrix(mat1.rows, mat1.cols, data)

