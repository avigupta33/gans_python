#include "../include/quantum.h"
#include "matrix.c"
// #include "propagate.c"
#include "random.c"

static PyModuleDef QuantumModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "Quantum Module",
    .m_doc = "#4145",
    .m_size = -1,
};

// (internal)
static PyObject* _quantum_type_error(const char *label, PyTypeObject *expected, PyTypeObject *actual) {
    return PyErr_Format(
        PyExc_TypeError,
        "%s must be type '%s', found type '%s'",
        label,
        expected->tp_name,
        actual->tp_name);
}

PyMODINIT_FUNC PyInit_Quantum() {
    static PyTypeObject *types[] = {
        &MatrixType,
        NULL
    };

    for (it i = 0; types[i] != NULL; ++i) {
        if (PyType_Ready(types[i]) < 0) return NULL;
    }

    PyObject *module = PyModule_Create(&QuantumModule);
    if (!module) return NULL;

    for (it i = 0; types[i] != NULL; ++i) {
        if (PyModule_AddObject(module, types[i]->tp_name, (PyObject*) types[i]) < 0) {
            Py_DECREF(module);
            return NULL;
        }
    }

    for (it i = 0; types[i] != NULL; ++i) {
        Py_INCREF(types[i]);
    }

    return module;
}