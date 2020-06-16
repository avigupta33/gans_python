import typing
from typing import *

class Matrix:
    def __init__(self, data: List[List[int]]) -> None:
        self.data = data
        self.rows = len(data)
        self.cols = len(data[0])

    def get(self, row: int, col:int ) -> int:
        return self.data[row][col]

    def set(self, row: int, col: int, value: int) -> None:
        self.data[row][col] = value