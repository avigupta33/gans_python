
from distutils.core import setup, Extension

module = Extension("Quantum", sources=['matrix.c'])

setup(name="Quantum Matrix",
      version="1,0",
      author="Quinn Okabayashi",
      author_email="qokabay1@swarthmore.edu",
      description="A package to compute matrix operations in neural networks",
      ext_modules=[module])