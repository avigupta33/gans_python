from Quantum import Matrix

# Create matrix from scalar
m = Matrix(0) # (1,1) matrix of zeros
m = Matrix(0, dims=(5,)) # (5,1) matrix of zeros
m = Matrix(0, dims=(5,3)) # (5,3) matrix of zeros
# m = Matrix(0, dims=(5,0)) # EXCEPTION

# Create matrix from list
m = Matrix([1,2,3]) # (3,1) matrix
m = Matrix([1,2,3], dims=(1,3)) # (1,3) matrix
m = Matrix([1,0,0,1], dims=(2,2)) # (2,2) matrix
# m = Matrix([1,2,3], dims=(2,2)) # EXCEPTION

# Create a matrix from a 2D list
m = Matrix([[1,0],[0,1]]) # (2,2) matrix
m = Matrix([[1,0],[0,1]], dims=(2,2)) # (2,2) matrix, but more specific
# m = Matrix([[1,0],[0,1]], dims=(4,1)) # EXCEPTION

# Copy a matrix by value
m = Matrix(m)

# Create a matrix from a random distribution
m = Matrix.gauss(mu=0, sigma=0.2, dims=(5,)) # (5,1) matrix from Gaussian distribution
m = Matrix.uniform(lower=-1, upper=1, dims=(5,)) # (5,1) matrix from uniform distribution

# A basic function wrapper that could be used
def vectorize(func):
    def wrapper(mat):
        # Uncomment following line to do map-by-reference instead
        # return mat.map(func)
        return Matrix(mat).map(func)
    return wrapper

def relu(x: float) -> float:
    return max(0, x)

a = Matrix.gauss(dims=(5,), seed=1)
b = Matrix.gauss(dims=(5,), seed=1)

print(a.T)
print(b.T)


# TODO: Add typing system to allow the user to choose what type to use
# options would be:
# u8, u16, u32, u64
# i8, i16, i32, i64
# f8, f16, f32, f64

print("Success!")