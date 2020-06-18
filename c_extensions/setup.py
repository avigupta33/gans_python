
from distutils.core import setup, Extension

module = Extension("example", sources=['example_module.c'])

setup(name="PackageName",
      version="1,0",
      description="This is a package for example",
      ext_modules=[module])