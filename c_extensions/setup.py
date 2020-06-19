
from distutils.core import setup, Extension

module = Extension("Matrix", sources=['matrix.c'])

setup(name="Quantum Matrix",
      version="1,0",
      description="Epic package for matrix operations",
      ext_modules=[module])