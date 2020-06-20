import timeit
from example import fib

def mySeq(target, arr):
    if target < 1:
        return arr[0]
    nextFib = sum(arr)
    arr[0] = arr[1]
    arr[1] = nextFib
    return mySeq(target-1, arr)

def myFib(n):
    arr = [0,1]
    return mySeq(n, arr)


def fibC():
    return fib(10000)


def fibPy():
    return myFib(1000)

if __name__ == "__main__":
    num = 1000
    print(timeit.timeit(fibC, number=num))
    # print(timeit.timeit(fibPy, number=num))