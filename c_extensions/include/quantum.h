#ifndef QUANTUM_H
#define QUANTUM_H

#include "python_path.h"

typedef double dtype;
typedef uint_fast64_t it;

static PyMethodDef QuantumMethodDefs[];
static PyModuleDef QuantumModule;

// module methods
static PyObject* quantum_seed(PyObject *self, PyObject *const *objs, Py_ssize_t nargs);

// (internal)
static PyObject* _quantum_type_error(const char *label, PyTypeObject *expected, PyTypeObject *actual);

PyMODINIT_FUNC PyInit_Quantum();

#endif /* QUANTUM_H */