
#ifndef MATRIX_H
#define MATRIX_H

#include <python3.7/Python.h>
#include "random.c"
#define QMatrix_Check(o) (Py_TYPE(o) == &MatrixType)

typedef double T;
typedef int(*compfunc)(T*, T*, T*, Py_ssize_t);
typedef struct MatrixObject {
    PyObject_HEAD
    long rows;
    long cols;
    Py_ssize_t size;
    T *data;
    PyObject *transpose;    // MatrixObject
    PyObject *Py_rows;      // PyLongObject
    PyObject *Py_cols;      // PyLongObject
    PyObject *Py_data;      // PyTupleObject (immutable to allow lazy construction)
    PyObject *Py_repr;      // PyUnicodeObject
} MatrixObject;


static RandomGenerator rg;

static PyTypeObject MatrixType;
static PyMethodDef MatrixMethodsDefs[];
static PyGetSetDef MatrixGetSetDefs[];
static PyNumberMethods MatrixNumberMethods;
static PyModuleDef QuantumModule;
PyMODINIT_FUNC PyInit_Quantum();

/* Non-MatrixObject functions */
static PyObject* zerosMatrix(PyObject *m, PyObject *args, PyObject *kwds);
static PyObject* fillMatrix(PyObject *m, PyObject *args, PyObject *kwds);
static PyObject* arrayMatrix(PyObject *m, PyObject *args, PyObject *kwds);
static PyObject* gaussMatrix(PyObject *m, PyObject *args, PyObject *kwds);
static PyObject* uniformMatrix(PyObject *m, PyObject *args, PyObject *kwds);

/* Custom C-only functions */
static MatrixObject* freshMatrix(long rows, long cols);
static int mallocMatrixData(MatrixObject *mat); // mallocs data and returns true on success, false on failure
static PyObject* unsupportedOperation(PyObject *a, PyObject *b, const char op); // sets err flag and returns NULL
static int loadMatrixDims(MatrixObject *self, PyObject *rows, PyObject *cols); // INCREFS rows/cols

/* Type functions */
static void Matrix_dealloc(MatrixObject *self);
static PyObject* Matrix_repr(MatrixObject *self, PyObject *Py_UNUSED(ignored));
static int Matrix_init(MatrixObject *self, PyObject *args, PyObject *kwds);
static PyObject* Matrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static void Matrix_clear (PyObject *mat);
static int Matrix_traverse(MatrixObject *self, visitproc visit, void *arg);

/* Get Set functions */
static PyObject* MatrixGet_rows(MatrixObject *self);
static PyObject* MatrixGet_cols(MatrixObject *self);
static PyObject* MatrixGet_data(MatrixObject *self);
static PyObject* MatrixGet_transpose(MatrixObject *self);

/* Numeric functions */
static PyObject* MatrixNumber_add(PyObject *a, PyObject *b);
static PyObject* MatrixNumber_subtract(PyObject *a, PyObject *b);
static PyObject* MatrixNumber_multiply(PyObject *a, PyObject *b);
static PyObject* MatrixNumber_divide(PyObject *a, PyObject *b);
static PyObject* MatrixNumber_matrix_multiply(PyObject *a, PyObject *b);

/* Numeric helper functions */
static PyObject* MatrixNumber_merge(MatrixObject *mat1, MatrixObject *mat2, compfunc compress);
static int MatrixNumber_merge_add(T *a, T *b, T *res, const Py_ssize_t size);
static int MatrixNumber_merge_subtract(T *a, T *b, T *res, const Py_ssize_t size);
static int MatrixNumber_merge_multiply(T *a, T *b, T *res, const Py_ssize_t size);
static int MatrixNumber_merge_divide(T *a, T *b, T *res, const Py_ssize_t size);
static PyObject* MatrixNumber_scalar_multiply(MatrixObject *mat, PyObject *scalar);
static PyObject* MatrixNumber_scalar_divide(MatrixObject *mat, PyObject *scalar);


#endif /* MATRIX_H */

