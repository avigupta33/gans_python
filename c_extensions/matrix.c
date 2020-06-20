
#include <python3.7/Python.h>
#include "quantum_types.h"
#include "iterstate.c"

typedef struct MatrixObject {
    PyObject_HEAD
    long rows;
    long cols;
    T *data;
    Py_ssize_t len;
    struct MatrixObject *transpose;
    PyLongObject *Py_rows;
    PyLongObject *Py_cols;
    PyTupleObject *Py_data; // Using tuple because it must be immutable so Avi can't mess it up
    PyUnicodeObject *Py_repr; 
} MatrixObject;


/* PyMethodDef functions */
static PyMethodDef MatrixMethodsDefs[] = {
    {NULL, NULL, 0, NULL}       /* Sentinal */
};


/* PyGetSetDef functions */
static PyObject* MatrixGet_rows(MatrixObject *self) {
    if (!self->Py_rows) {
        self->Py_rows = PyLong_FromLong(self->rows);
        if (self->Py_rows) {
            Py_INCREF(self->Py_rows);
        } else {
            return PyErr_NoMemory();
        }
    }
    return self->Py_rows;
}

static PyObject* MatrixGet_cols(MatrixObject *self) {
    if (!self->Py_cols) {
        self->Py_cols = PyLong_FromLong(self->cols);
        if (self->Py_cols) {
            Py_INCREF(self->Py_cols);
        } else {
            return PyErr_NoMemory();
        }
    }
    return self->Py_cols;
}

static PyObject* MatrixGet_data(MatrixObject *self) {
    if (!self->Py_data) {
        self->Py_data = PyTuple_New(self->len);
        if (self->Py_data) {
            Py_INCREF(self->Py_data);
        } else {
            return PyErr_NoMemory();
        }
        for (Py_ssize_t i = 0; i < self->len; ++i) {
            PyObject *item = PyFloat_FromDouble(self->data[i]);
            Py_INCREF(item);
            PyTuple_SET_ITEM(self->Py_data, i, item);
        }
    }
    return self->Py_data;
}

static PyObject* MatrixGet_transpose(MatrixObject *self) {
    // if (!self->transpose) {
    //     self->transpose = Matrix_new(&MatrixType, NULL, NULL);
    //     // build the tranpose object
    // }
    // return self->transpose;
    PyErr_SetString(PyExc_NotImplementedError, "still need to figure out cyclic GC");
    return NULL;
}

static PyGetSetDef MatrixGetSetDefs[] = {
    {"rows", (getter) MatrixGet_rows, NULL, 
     "Get rows"},
    {"cols", (getter) MatrixGet_cols, NULL, 
     "Get cols"},
    {"data", (getter) MatrixGet_data, NULL, 
     "Get data"},
    {NULL, NULL, NULL, NULL}    /* Sentinal */
};


/* PyTypeObject functions */
static PyObject* Matrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    MatrixObject *self = (MatrixObject*) type->tp_alloc(type, 0);
    if (!self) return PyErr_NoMemory();

    self->rows = 0;
    self->cols = 0;
    self->data = NULL;
    self->len = 0;
    self->transpose = NULL;
    self->Py_rows = NULL;
    self->Py_cols = NULL;
    self->Py_data = NULL;
    self->Py_repr = NULL;
    return (PyObject*) self;
}

static int Matrix_init(MatrixObject *self, PyObject *args, PyObject *kwds) {
    static const char *kwlist[] = {"rows", "cols", "data", NULL};
    PyObject *input_data = NULL;

    // Parse args
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "llO", kwlist, &self->rows, &self->cols, &input_data)) {
        PyErr_SetString(PyExc_ValueError, "unable to parse row and col args");
        return -1;
    }

    // Check that rows and cols are positive
    if (self->rows <= 0 || self->cols <= 0) {
        PyErr_Format(PyExc_ValueError, "matrix dims must be non-zero positive integers: (%ld, %ld)", self->rows, self->cols);
        return -1;
    }

    self->len = self->rows * self->cols;

    // Check that a list was received
    if (!PyList_Check(input_data)) {
        PyErr_Format(PyExc_TypeError, "cannot construct data from object of type '%s'", Py_TYPE(input_data)->tp_name);
        return -1;
    }

    Py_ssize_t data_size = PyList_Size(input_data);

    // Check for invalid number of elements received
    if (self->len != data_size) {
        PyErr_Format(PyExc_ValueError, "expected %ld elements in data, but received %ld", self->len, data_size);
        return -1;
    }


    if (self->data) free(self->data);
    self->data = (T*) malloc(sizeof(T) * self->len);
    if (!self->data) {
        PyErr_NoMemory();
        return -1;
    }
    for (int i = 0; i < self->len; ++i) {
        PyObject *item = PyList_GetItem(self->Py_data, i);
        if (!PyLong_Check(item)) return -1; // TODO proper error
        self->data[i] = PyFloat_AsDouble(item);
    }
    return 0;
}

static void Matrix_dealloc(MatrixObject *self) {
    // TRANSPOSE DEALLOC FRAMEWORK
    // if (self->transpose) {
    //     // Has transpose
    //     if (self->transpose->data != self->data) {
    //         // self data isn't shared with transpose
    //         Py_DECREF(self->Py_data);
    //         free(self->data);
    //     }
    //         // Tell transpose to forget about me
    // } else {
    //     // No transpose
    //     Py_DECREF(self->Py_rows);
    //     Py_DECREF(self->Py_cols);
    // }
    if (self->data) free(self->data);
    if (self->Py_rows) Py_DECREF(self->Py_rows);
    if (self->Py_cols) Py_DECREF(self->Py_cols);
    if (self->Py_data) Py_DECREF(self->Py_data);
    if (self->Py_repr) Py_DECREF(self->Py_repr);

    Py_TYPE(self)->tp_free((PyObject*) self);
}

static PyObject* Matrix_repr(MatrixObject *self, PyObject *Py_UNUSED(ignored)) {
    if (!self->Py_repr) {
        self->Py_repr = PyUnicode_FromFormat("Matrix with %d rows and %d cols", self->rows, self->cols);
        if (self->Py_repr) {
            Py_INCREF(self->Py_repr);
        } else {
            return PyErr_NoMemory();
        }
    }
    return self->Py_repr;
}

static PyTypeObject MatrixType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "Matrix",
    .tp_doc = "Matrix class in C",
    .tp_basicsize = sizeof(MatrixObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = Matrix_new,
    .tp_init = (initproc) Matrix_init,
    .tp_dealloc = (destructor) Matrix_dealloc,
    .tp_methods = MatrixMethodsDefs,
    .tp_repr = (reprfunc) Matrix_repr,
    .tp_getset = MatrixGetSetDefs
};


/* PyNumberMethods functions */
static PyObject* C_Matrix_new(long rows, long  cols, T *unordered_data) {
    MatrixObject *self = (MatrixObject*) Matrix_new(&MatrixType, NULL, NULL);
    self->rows = rows;
    self->cols = cols;
    self->len = rows * cols;
    self->unordered_data = unordered_data;
    return (PyObject*) self;
}

static PyObject* unsupported_err(PyObject *a, PyObject *b, const char op) {
    PyErr_Format(PyExc_TypeError, "unsupported operand type(s) for %c: '%s' and '%s'", op, a->ob_type->tp_name, b->ob_type->tp_name);
    return NULL;
}

static PyObject* MatrixNumber_merge(MatrixObject *self, PyObject *o, compfunc compress) {
    if (!PyObject_TypeCheck(o, &MatrixType)) {
        return unsupported_err((PyObject*) self, o, '+');
    }
    MatrixObject *m = (MatrixObject*) o;
    if (self->rows != m->rows || self->cols != m->cols) {
        PyErr_SetString(PyExc_ValueError, "Matrices are not the same shape");
        return NULL;
    }
    T *ret_data = (T*) malloc(sizeof(T) * self->len);
    if (!ret_data) return PyErr_NoMemory();
    compress(self->unordered_data, m->unordered_data, ret_data, self->len);
    return C_Matrix_new(self->rows, self->cols, ret_data);
}

static void MatrixNumber_merge_add(T *a, T *b, T *res, Py_ssize_t len) {
    for (Py_ssize_t i = 0; i < len; ++i) {
        *res = *a + *b;
        ++a;
        ++b;
        ++res;
    }
}

static void MatrixNumber_merge_subtract(T *a, T *b, T *res, Py_ssize_t len) {
    for (Py_ssize_t i = 0; i < len; ++i) {
        *res = *a - *b;
        ++a;
        ++b;
        ++res;
    }
}

static void MatrixNumber_merge_multiply(T *a, T *b, T *res, Py_ssize_t len) {
    for (Py_ssize_t i = 0; i < len; ++i) {
        *res = *a * *b;
        ++a;
        ++b;
        ++res;
    }
}

static PyObject* MatrixNumber_add(MatrixObject *self, PyObject *o) {
    return MatrixNumber_merge(self, o, MatrixNumber_merge_add);
}

static PyObject* MatrixNumber_subtract(MatrixObject *self, PyObject *o) {
    return MatrixNumber_merge(self, o, MatrixNumber_merge_subtract);
}

static PyObject* MatrixNumber_multiply(MatrixObject *self, PyObject *o) {
    return MatrixNumber_merge(self, o, MatrixNumber_merge_multiply);
}

static PyObject* MatrixNumber_matrix_multiply(MatrixObject *self, PyObject *o) {
    if (!PyObject_TypeCheck(o, &MatrixType)) {
        return unsupported_err((PyObject*) self, o, '@');
    }
    return Py_BuildValue("s", "Matrix multiplication");
}

static PyNumberMethods MatrixNumberMethods = {
    .nb_add = (binaryfunc) MatrixNumber_add,
    .nb_subtract = (binaryfunc) MatrixNumber_subtract,
    .nb_multiply = (binaryfunc) MatrixNumber_multiply,
    .nb_matrix_multiply = (binaryfunc) MatrixNumber_matrix_multiply,
};


/* Module definition with methods */
static PyModuleDef QuantumModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "Quantum Module",
    .m_doc = "#4145",
    .m_size = -1,
};


PyMODINIT_FUNC PyInit_Quantum() {
    // Any methods that needs to typecheck if something is a MatrixType
    // needs to be added here (and not in the construction of MatrixType)
    MatrixType.tp_as_number = &MatrixNumberMethods;
    if (PyType_Ready(&MatrixType) < 0) return NULL;

    PyObject *module = PyModule_Create(&QuantumModule);

    if (!module) return NULL;

    Py_INCREF(&MatrixType);
    if (PyModule_AddObject(module, "Matrix", (PyObject*) &MatrixType) < 0) {
        Py_DECREF(&MatrixType);
        Py_DECREF(module);
        return NULL;
    }
    return module;

}
