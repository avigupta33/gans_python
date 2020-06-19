
#include <python3.7/Python.h>


typedef struct {
    PyObject_HEAD
    unsigned int rows;
    unsigned int cols;
    size_t len;
    float *unordered_data;
} MatrixObject;


static void Matrix_dealloc(MatrixObject *self) {
    free(self->unordered_data);
    Py_TYPE(self)->tp_free((PyObject*) self);
}


static PyObject* Matrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    MatrixObject *self;
    self = (MatrixObject*) type->tp_alloc(type, 0);
    self->rows = 0;
    self->cols = 0;
    self->len = 0;
    self->unordered_data = NULL;
    return (PyObject*) self;
}


static PyObject* Matrix_init(MatrixObject *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"rows", "cols", "data", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "ii|", kwlist, &self->rows, &self->cols)) {
        return -1;
    }
}


static PyTypeObject MatrixType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "Matrix",
    .tp_doc = "Matrix class in C",
    .tp_basicsize = sizeof(MatrixObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = (destructor) Matrix_dealloc,
};


/* Method table */
static PyMethodDef matrixMethods[] = {
    // {
    //     "fib",
    //     example_fib,
    //     METH_VARARGS,
    //     "A method to get the nth value of the Fibonacci sequence"
    // },
    {NULL, NULL, 0, NULL}                   /* Required null-terminating method */
};


/* Module definition with methods */
static PyModuleDef exampleModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "example",
    .m_doc = "An example module",
    .m_size = -1,
    matrixMethods
};


PyMODINIT_FUNC PyInit_Matrix() {
    if (PyType_Ready(&MatrixType) < 0) return NULL;

    PyObject *m = PyModule_Create(&exampleModule);

    if (!m) return NULL;

    Py_INCREF(&MatrixType);
    if (PyModule_AddObject(m, "Matrix", (PyObject*) &MatrixType) < 0) {
        Py_DECREF(&MatrixType);
        Py_DECREF(m);
        return NULL;
    }
    return m;

}
