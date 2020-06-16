import typing
from typing import *

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

    @classmethod
    def zeros(cls, rows, cols):
        return cls([[0 for _ in range(cols)] for _ in range(rows)])