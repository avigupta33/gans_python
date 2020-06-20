
#include <python3.7/Python.h>
#include "quantum_types.h"
#include "iterstate.c"

typedef struct {
    PyObject_HEAD
    long rows;
    long cols;
    T *unordered_data;
    Py_ssize_t len;
    IterState *rowIter;
    IterState *colIter;
    PyObject *Py_rows;
    PyObject *Py_cols;
    PyObject *Py_data;
    /*
     some sort of struct that will let me iterate
    */
    
} MatrixObject;


/* PyMethodDef functions */

static PyMethodDef MatrixMethodsDefs[] = {
    {NULL, NULL, 0, NULL}       /* Sentinal */
};


/* PyGetSetDef functions */
static PyObject* MatrixGetSet_getRows(MatrixObject *self) {
    return PyLong_FromLong(self->rows);
}

static PyObject* MatrixGetSet_getCols(MatrixObject *self) {
    return PyLong_FromLong(self->cols);
}

static PyObject* MatrixGetSet_getData(MatrixObject *self) {
    PyObject *list = PyList_New(self->len);
    Py_INCREF(list);
    for (Py_ssize_t i = 0; i < self->len; ++i) {
        PyObject *item = PyFloat_FromDouble(self->unordered_data[i]);
        Py_INCREF(item);
        PyList_SET_ITEM(list, i, item);
    }
    return list;
}

static PyGetSetDef MatrixGetSetDefs[] = {
    {"rows", (getter) MatrixGetSet_getRows, NULL, 
     "Get rows"},
    {"cols", (getter) MatrixGetSet_getCols, NULL, 
     "Get cols"},
    {"data", (getter) MatrixGetSet_getData, NULL, 
     "Get data"},
    {NULL, NULL, NULL, NULL}    /* Sentinal */
};


/* PyTypeObject functions */
static PyObject* Matrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    MatrixObject *self = (MatrixObject*) type->tp_alloc(type, 0);
    if (self) {
        self->rows = 0;
        self->cols = 0;
        self->len = 0;
        self->unordered_data = NULL;
    }
    return (PyObject*) self;
}

static int Matrix_init(MatrixObject *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"rows", "cols", "data", NULL};
    PyObject *data = NULL;

    // Parse args
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "llO", kwlist, &self->rows, &self->cols, &data)) {
        return -1;
    }

    // Check that rows and cols are positive
    if (self->rows <= 0 || self->cols <= 0) return -1;

    // Check that a list was received
    if (!PyList_Check(data)) {
        PyErr_Format(PyExc_TypeError, "cannot construct data from object of type '%s'", data->ob_type->tp_name);
        return -1;
    }

    self->len = PyList_Size(data);

    // Check for invalid list length received
    if (self->len != self->rows * self->cols) return -1;// TODO: figure out how to raise as python exception

    if (self->unordered_data) free(self->unordered_data);
    self->unordered_data = (T*) malloc(sizeof(T) * self->len);
    if (!self->unordered_data) {
        PyErr_NoMemory();
        return -1;
    }
    for (int i = 0; i < self->len; ++i) {
        PyObject *item = PyList_GetItem(data, i);
        if (!PyLong_Check(item)) return -1; // TODO proper error
        self->unordered_data[i] = PyFloat_AsDouble(item);
        // printf("%f\n", self->unordered_data[i]);
    }
    return 0;
}

static void Matrix_dealloc(MatrixObject *self) {
    free(self->unordered_data);
    Py_TYPE(self)->tp_free((PyObject*) self);
}

static PyObject* Matrix_repr(MatrixObject *self, PyObject *Py_UNUSED(ignored)) {
    return PyUnicode_FromFormat("Matrix with %d rows and %d cols", self->rows, self->cols);
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

typedef void(*compfunc)(T*, T*, T*, Py_ssize_t);

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
