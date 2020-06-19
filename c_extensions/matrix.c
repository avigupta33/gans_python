
#include <python3.7/Python.h>


typedef struct {
    PyObject_HEAD
    long rows;
    long cols;
    Py_ssize_t len;
    double *unordered_data;
} MatrixObject;


/* PyMethodDef functions */
static PyObject* Matrix_getRows(MatrixObject *self) {
    return PyLong_FromLong(self->rows);
}

static PyObject* Matrix_getCols(MatrixObject *self) {
    return PyLong_FromLong(self->cols);
}

static PyMethodDef MatrixMethodsDefs[] = {
    {"getRows", (PyCFunction) Matrix_getRows, METH_NOARGS,
     "Get number of rows"},
    {"getCols", (PyCFunction) Matrix_getCols, METH_NOARGS,
     "Get number of cols"},
    {NULL, NULL, 0, NULL}       /* Sentinal */
};


/* PyTypeObject functions */
static PyObject* Matrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    MatrixObject *self;
    self = (MatrixObject*) type->tp_alloc(type, 0);
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
    if (!PyList_Check(data)) return -1;

    self->len = PyList_Size(data);

    // Check for invalid list length received
    if (self->len != self->rows * self->cols) return -1;// TODO: figure out how to raise as python exception

    if (self->unordered_data) free(self->unordered_data);
    self->unordered_data = (double*) malloc(sizeof(double) * self->len);
    
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
};


/* PyNumberMethods functions */
static PyObject* MatrixNumber_add(MatrixObject *self, PyObject *m) {
    if (Py_TYPE(m) != &MatrixType) {
        return Py_BuildValue("s", "chief this is not epic...");
    }
    return Py_BuildValue("s", "Element-wise addition");
}

static PyObject* MatrixNumber_subtract(MatrixObject *self, PyObject *m) {
    return Py_BuildValue("s", "Element-wise subtraction");
}

static PyObject* MatrixNumber_multiply(MatrixObject *self, PyObject *m) {
    return Py_BuildValue("s", "Element-wise multiplication");
}

static PyObject* MatrixNumber_matrix_multiply(MatrixObject *self, PyObject *m) {
    return Py_BuildValue("s", "Matrix multiplication");
}

static PyNumberMethods MatrixNumberMethods = {
    .nb_add = (binaryfunc) MatrixNumber_add,
    .nb_subtract = (binaryfunc) MatrixNumber_subtract,
    .nb_multiply = (binaryfunc) MatrixNumber_multiply,
    .nb_matrix_multiply = (binaryfunc) MatrixNumber_matrix_multiply,
};


/* Module definition with methods */
static PyModuleDef MatrixModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "Matrix",
    .m_doc = "An efficient matrix class for ML",
    .m_size = -1,
};


PyMODINIT_FUNC PyInit_Matrix() {
    // Any methods that needs to typecheck if something is a MatrixType
    // needs to be added here (and not in the construction of MatrixType)
    MatrixType.tp_as_number = &MatrixNumberMethods;
    if (PyType_Ready(&MatrixType) < 0)
        return NULL;

    PyObject *m = PyModule_Create(&MatrixModule);

    if (!m) return NULL;

    Py_INCREF(&MatrixType);
    if (PyModule_AddObject(m, "Matrix", (PyObject*) &MatrixType) < 0) {
        Py_DECREF(&MatrixType);
        Py_DECREF(m);
        return NULL;
    }
    return m;

}
