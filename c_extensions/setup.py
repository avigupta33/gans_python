
from distutils.core import setup, Extension

module = Extension("Matrix", sources=['matrix.c'])

setup(name="PackageName",
      version="1,0",
      description="This is a package for example",
      ext_modules=[module])