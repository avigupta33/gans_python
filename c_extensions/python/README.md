# 1. Why do I need a pypath.h?

The Quantum module is a compiled library, and each compiled version is specific to the machine it was compiled on.

To build the module on your machine, your computer must have python developer tools installed (see below)

`pypath.h` allows the module to include a general filepath, so the user doesn't have to modify internal code

# 2. Installing Python dev tools

To compile the Quantum module, you need to install the python developer toolkit

## **Linux**:
The following command may or may not work. Good luck!

`sudo apt-get install python3.8-dev`

## **OSX**:
Installing Python through Homebrew automatically installs the dev tools

Homebrew installations instructions can be found here: [https://docs.python-guide.org/starting/install3/osx/](https://docs.python-guide.org/starting/install3/osx/)

## **Windows**:
1. Bootcamp your computer to Linux
2. Follow Linux instructions
3. Seriously, why.

# 3. Creating pypath.h

In `gans_python/c_extensions/python/`, create a file named `pypath.h`

**(Linux/OSX):** It may be useful to run `locate Python.h` in the terminal to find your `Python.h`

Paste the following contents into the file, substituting your include path:

```c
#ifndef PYPATH_H
#define PYPATH_H

#include <path/to/your/Python.h>

#endif /* PYPATH_H */
```
