
#ifndef MATRIX_H
#define MATRIX_H

#include <python3.7/Python.h>
#include "random.c"
#define Matrix_Check(obj) (Py_TYPE(obj) == &MatrixType)


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
static PyMethodDef QuantumMethodDefs[];
static PyModuleDef QuantumModule;
PyMODINIT_FUNC PyInit_Quantum();

/* Non-MatrixObject functions */
static PyObject* matrix_from_zeros(PyObject *m, PyObject *args, PyObject *kwds);
static PyObject* matrix_from_value(PyObject *m, PyObject *args, PyObject *kwds);
static PyObject* matrix_from_array(PyObject *m, PyObject *args, PyObject *kwds);
static PyObject* matrix_from_gauss(PyObject *m, PyObject *args, PyObject *kwds);
static PyObject* matrix_from_uniform(PyObject *m, PyObject *args, PyObject *kwds);

/* Custom C-only functions */
static MatrixObject* freshMatrix(long rows, long cols);
static T* matrix_alloc_data(MatrixObject *mat); // mallocs data and returns true on success, false on failure
static PyObject* unsupportedOperation(PyObject *a, PyObject *b, const char op); // sets err flag and returns NULL
static int matrix_load_dims(MatrixObject *self, PyObject *rows, PyObject *cols); // INCREFS rows/cols

/* Type functions */
static void matrix_dealloc(MatrixObject *self);
static PyObject* matrix_repr(MatrixObject *self, PyObject *Py_UNUSED(ignored));
static int matrix_init(MatrixObject *self, PyObject *args, PyObject *kwds);
static PyObject* matrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static void matrix_clear (PyObject *mat);
static int matrix_traverse(MatrixObject *self, visitproc visit, void *arg);

/* Get Set functions */
static PyObject* matrix_get_rows(MatrixObject *self);
static PyObject* matrix_get_cols(MatrixObject *self);
static PyObject* matrix_get_data(MatrixObject *self);
static PyObject* matrix_get_transpose(MatrixObject *self);

/* Numeric functions */
static PyObject* matrix_number_add(PyObject *obj1, PyObject *obj2);
static PyObject* matrix_number_subtract(PyObject *obj1, PyObject *obj2);
static PyObject* matrix_number_multiply(PyObject *obj1, PyObject *obj2);
static PyObject* matrix_number_divide(PyObject *obj1, PyObject *obj2);
static PyObject* matrix_number_matmul(PyObject *obj1, PyObject *obj2);

/* Numeric helper functions */
static PyObject* MatrixNumber_merge(MatrixObject *mat1, MatrixObject *mat2, compfunc compress);
static int MatrixNumber_merge_add(T *a, T *b, T *res, const Py_ssize_t size);
static int MatrixNumber_merge_subtract(T *a, T *b, T *res, const Py_ssize_t size);
static int MatrixNumber_merge_multiply(T *a, T *b, T *res, const Py_ssize_t size);
static int MatrixNumber_merge_divide(T *a, T *b, T *res, const Py_ssize_t size);
static PyObject* MatrixNumber_scalar_multiply(MatrixObject *mat, PyObject *scalar);
static PyObject* MatrixNumber_scalar_divide(MatrixObject *mat, PyObject *scalar);

/* Quantum method functions */
static PyObject* Quantum_sigmoid_forwards(PyObject *self, PyObject *const *objs, Py_ssize_t nargs);
static PyObject* Quantum_sigmoid_backwards(PyObject *self, PyObject *const *objs, Py_ssize_t nargs);
static PyObject* Quantum_relu_forwards(PyObject *self, PyObject *const *objs, Py_ssize_t nargs);
static PyObject* Quantum_relu_backwards(PyObject *self, PyObject *const *objs, Py_ssize_t nargs);
static PyObject* Quantum_leakyrelu_forwards(PyObject *self, PyObject *const *objs, Py_ssize_t nargs);
static PyObject* Quantum_leakyrelu_backwards(PyObject *self, PyObject *const *objs, Py_ssize_t nargs);
static PyObject* Quantum_tanh_forwards(PyObject *self, PyObject *const *objs, Py_ssize_t nargs);
static PyObject* Quantum_tanh_backwards(PyObject *self, PyObject *const *objs, Py_ssize_t nargs);

/* Quantum helper functions */
static PyObject* activation(PyObject *o, void(*map)(T*,T*));
static inline void sigmoid_forwards(T *x, T *y);
static inline void sigmoid_backwards(T *dy, T *dx);
static inline void relu_forwards(T *x, T *y);
static inline void relu_backwards(T *dy, T *dx);
static inline void leakyrelu_forwards(T *x, T *y);
static inline void leakyrelu_backwards(T *dy, T *dx);
static inline void tanh_forwards(T *x, T *y);
static inline void tanh_backwards(T *dy, T *dx);

#endif /* MATRIX_H */

