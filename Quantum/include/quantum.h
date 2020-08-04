#ifndef QUANTUM_H
#define QUANTUM_H

#include "pypath.h"

typedef double dtype;
typedef uint_fast64_t it;

static PyModuleDef QuantumModule;

// (internal)
static PyObject* _quantum_type_error(const char *label, PyTypeObject *expected, PyTypeObject *actual);

PyMODINIT_FUNC PyInit_Quantum();

#endif /* QUANTUM_H */