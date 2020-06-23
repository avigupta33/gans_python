
#include <python3.7/Python.h>
#include "matrix.h"

/* Non-MatrixObject functions */
static PyObject* zerosMatrix(PyObject *m, PyObject *args, PyObject *kwds) {
    MatrixObject *self = (MatrixObject*) Matrix_new(&MatrixType, NULL, NULL);
    static char *kwlist[] = {"rows", "cols", NULL};
    PyObject *rows = NULL;
    PyObject *cols = NULL;

    // Parse args
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OO", kwlist, &rows, &cols)) {
        PyErr_SetString(PyExc_ValueError, "unable to parse parameters");
        return NULL;
    }
    // printf("(%ld, %ld)", PyLong_AsLong(rows), PyLong_AsLong(cols));

    if (!loadMatrixDims(self, rows, cols)) return NULL;

    self->data = (T*) calloc(self->size, sizeof(T));
    if (!self->data) return PyErr_NoMemory();

    Py_INCREF(self);
    return (PyObject*) self;
}

static PyObject* fillMatrix(PyObject *m, PyObject *args, PyObject *kwds) {
    MatrixObject *self = (MatrixObject*) Matrix_new(&MatrixType, NULL, NULL);
    static char *kwlist[] = {"rows", "cols", "val", NULL};
    PyObject *rows = NULL;
    PyObject *cols = NULL;
    double val = 0;

    // Parse args
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OOd", kwlist, &rows, &cols, &val)) return NULL;
    if (!loadMatrixDims(self, rows, cols)) return NULL;

    self->data = (T*) malloc(sizeof(T) * self->size);
    if (!self->data) return PyErr_NoMemory();

    for (Py_ssize_t i = 0; i < self->size; ++i) {
        self->data[i] = val;
    }

    Py_INCREF(self);
    return (PyObject*) self;
}

static PyObject* arrayMatrix(PyObject *m, PyObject *args, PyObject *kwds) {
    MatrixObject *self = (MatrixObject*) Matrix_new(&MatrixType, NULL, NULL);
    static char *kwlist[] = {"rows", "cols", "data", NULL};
    PyObject *input_data = NULL;
    PyObject *rows = NULL;
    PyObject *cols = NULL;

    // Parse args
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OOO", kwlist, &rows, &cols, &input_data)) return NULL;
    if (!loadMatrixDims(self, rows, cols)) return NULL;

    // Check that a list was received
    if (!PyList_Check(input_data)) {
        PyErr_Format(PyExc_TypeError, "cannot construct data from object of type '%s'", Py_TYPE(input_data)->tp_name);
        return NULL;
    }

    Py_ssize_t data_size = PyList_Size(input_data);

    // Check for invalid number of elements received
    if (self->size != data_size) {
        PyErr_Format(PyExc_ValueError, "expected %ld element(s) in data, but received %ld", self->size, data_size);
        return NULL;
    }

    if (!mallocMatrixData(self)) return NULL;

    for (Py_ssize_t i = 0; i < self->size; ++i) {
        PyObject *item = PyList_GetItem(input_data, i);
        if (!PyLong_Check(item)) {
            PyErr_Format(PyExc_TypeError, "encounted non-numeric type in data: %s", Py_TYPE(item)->tp_name);
            return NULL;
        }
        self->data[i] = PyFloat_AsDouble(item);
    }
    return (PyObject*) self;
}

static PyObject* gaussMatrix(PyObject *m, PyObject *args, PyObject *kwds) {
    MatrixObject *self = (MatrixObject*) Matrix_new(&MatrixType, NULL, NULL);
    static char *kwlist[] = {"rows", "cols", "mu", "sigma", NULL};
    PyObject *rows = NULL;
    PyObject *cols = NULL;
    double mu = 0;
    double sigma = 0;

    // Parse args
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OOdd", kwlist, &rows, &cols, &mu, &sigma)) return NULL;
    if (!loadMatrixDims(self, rows, cols)) return NULL;

    self->data = (T*) malloc(sizeof(T) * self->size);
    if (!self->data) return PyErr_NoMemory();

    for (Py_ssize_t i = 0; i < self->size; ++i) {
        self->data[i] = gauss(&rg, mu, sigma);
    }

    Py_INCREF(self);
    return (PyObject*) self;
}

static PyObject* uniformMatrix(PyObject *m, PyObject *args, PyObject *kwds) {
    MatrixObject *self = (MatrixObject*) Matrix_new(&MatrixType, NULL, NULL);
    static char *kwlist[] = {"rows", "cols", "lower_bound", "upper_bound", NULL};
    PyObject *rows = NULL;
    PyObject *cols = NULL;
    double lower_bound = 0;
    double upper_bound = 0;

    // Parse args
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OOdd", kwlist, &rows, &cols, &lower_bound, &upper_bound)) return NULL;
    if (!loadMatrixDims(self, rows, cols)) return NULL;

    self->data = (T*) malloc(sizeof(T) * self->size);
    if (!self->data) return PyErr_NoMemory();

    for (Py_ssize_t i = 0; i < self->size; ++i) {
        self->data[i] = uniformInRange(lower_bound, upper_bound);
    }

    Py_INCREF(self);
    return (PyObject*) self;
}


/* Custom C MatrixObject functions */
static MatrixObject* freshMatrix(long rows, long cols) {
    MatrixObject *self = (MatrixObject*) Matrix_new(&MatrixType, NULL, NULL);
    if (!self) return NULL;
    self->rows = rows;
    self->cols = cols;
    self->size = rows * cols;
    if (!mallocMatrixData(self)) return NULL;
    return self;
}

static int mallocMatrixData(MatrixObject *mat) {
    mat->data = (T*) malloc(sizeof(T) * mat->size);
    if (!mat->data) {
        PyErr_NoMemory();
        return 0;
    }
    return 1;
}

static PyObject* unsupportedOperation(PyObject *a, PyObject *b, const char op) {
    PyErr_Format(PyExc_TypeError, "unsupported operand type(s) for %c: '%s' and '%s'", op, Py_TYPE(a)->tp_name, Py_TYPE(b)->tp_name);
    return Py_NotImplemented;
}

static int loadMatrixDims(MatrixObject *self, PyObject *rows, PyObject *cols) {
    // printf("(%ld, %ld)", PyLong_AsLong(rows), PyLong_AsLong(cols));
    // Load rows from PyObject
    if (!PyLong_Check(rows)) {
        PyErr_Format(PyExc_TypeError, "rows argument must be of type 'int', received type '%s'", Py_TYPE(self->Py_rows)->tp_name);
        return 0;
    }
    self->Py_rows = rows;
    self->rows = PyLong_AsLong(self->Py_rows);

    // Load cols from PyObject
    if (!PyLong_Check(cols)) {
        PyErr_Format(PyExc_TypeError, "cols argument must be of type 'int', received type '%s'", Py_TYPE(self->Py_cols)->tp_name);
        return 0;
    }
    self->Py_cols = cols;
    self->cols = PyLong_AsLong(self->Py_cols);

    // Check that rows and cols are positive
    if (self->rows <= 0 || self->cols <= 0) {
        PyErr_Format(PyExc_ValueError, "matrix dims must be non-zero positive integers: (%ld, %ld)", self->rows, self->cols);
        return 0;
    }
    /* Only call Py_INCREF at end when we know the dims have loaded successfully */
    self->size = self->rows * self->cols;
    Py_INCREF(rows);
    Py_INCREF(cols);
    return 1;
}

/* MatrixMethodDef functions */
static PyMethodDef MatrixMethodsDefs[] = {
    {"zeros", (PyCFunction) zerosMatrix, METH_CLASS | METH_VARARGS | METH_KEYWORDS, 
     "Get a matrix filled with zeros"},
    {"fill", (PyCFunction) fillMatrix, METH_CLASS | METH_VARARGS | METH_KEYWORDS, 
     "Get a matrix filled with constant scalar value"},
    {"array", (PyCFunction) arrayMatrix, METH_CLASS | METH_VARARGS | METH_KEYWORDS, 
     "Get a matrix from a flattened array"},
    {"gauss", (PyCFunction) gaussMatrix, METH_CLASS | METH_VARARGS | METH_KEYWORDS, 
     "Get a matrix filled with random numbers from a gaussian distribution"},
    {"uniform", (PyCFunction) uniformMatrix, METH_CLASS | METH_VARARGS | METH_KEYWORDS, 
     "Get a matrix filled with uniformally distributed random numbers between bounds"},
    {NULL, NULL, 0, NULL}       /* Sentinal */
};


/* PyGetSetDef functions */
static PyObject* MatrixGet_rows(MatrixObject *self) {
    if (!self->Py_rows) {
        self->Py_rows = PyLong_FromLong(self->rows);
        if (!self->Py_rows) return PyErr_NoMemory();
    }
    Py_INCREF(self->Py_rows);
    return self->Py_rows;
}

static PyObject* MatrixGet_cols(MatrixObject *self) {
    if (!self->Py_cols) {
        self->Py_cols = PyLong_FromLong(self->cols);
        if (!self->Py_cols) return PyErr_NoMemory();
    }
    Py_INCREF(self->Py_cols);
    return self->Py_cols;
}

static PyObject* MatrixGet_data(MatrixObject *self) {
    if (!self->Py_data) {
        self->Py_data = PyTuple_New(self->size);
        if (!self->Py_data) return PyErr_NoMemory();
        for (Py_ssize_t i = 0; i < self->size; ++i) {
            PyObject *item = PyFloat_FromDouble(self->data[i]);
            if (item) {
                Py_INCREF(item);
            } else {
                return PyErr_NoMemory();
            }
            PyTuple_SET_ITEM(self->Py_data, i, item);
        }
    }
    Py_INCREF(self->Py_data);
    return self->Py_data;
}

static PyObject* MatrixGet_transpose(MatrixObject *self) {
    if (!self->transpose) {
        MatrixObject *trans = (MatrixObject*) Matrix_new(&MatrixType, NULL, NULL);
        trans->rows = self->cols;
        trans->cols = self->rows;
        trans->Py_rows = self->Py_cols;
        trans->Py_cols = self->Py_rows;
        Py_INCREF(trans->Py_rows);
        Py_INCREF(trans->Py_cols);
        trans->size = trans->rows * trans->cols;
        if (!mallocMatrixData(trans)) return NULL;

        // transpose algo
        T *self_ptr = self->data;
        T *trans_ptr = trans->data;
        for (long self_row_i = 0; self_row_i < self->rows; ++self_row_i) {
            for (long self_col_i = 0; self_col_i < self->cols; ++self_col_i) {
                *trans_ptr = *self_ptr;
                trans_ptr += trans->cols;
                ++self_ptr;
            }
            trans_ptr -= trans->size - 1;
        }

        trans->transpose = (PyObject*) self;
        self->transpose = (PyObject*) trans;
        Py_INCREF(self);
    }
    Py_INCREF(self->transpose);
    return self->transpose;
}

static PyGetSetDef MatrixGetSetDefs[] = {
    {"rows", (getter) MatrixGet_rows, NULL, 
     "Get rows"},
    {"cols", (getter) MatrixGet_cols, NULL, 
     "Get cols"},
    {"data", (getter) MatrixGet_data, NULL, 
     "Get data"},
    {"T", (getter) MatrixGet_transpose, NULL, 
     "Get transpose"},
    {NULL, NULL, NULL, NULL}    /* Sentinal */
};


/* PyTypeObject functions */
static PyObject* Matrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    MatrixObject *self = (MatrixObject*) type->tp_alloc(type, 0);
    if (!self) return PyErr_NoMemory();
    self->rows = 0;
    self->cols = 0;
    self->size = 0;
    self->data = NULL;
    self->transpose = NULL;
    self->Py_rows = NULL;
    self->Py_cols = NULL;
    self->Py_data = NULL;
    self->Py_repr = NULL;
    Py_INCREF(self);
    return (PyObject*) self;
}

static int Matrix_init(MatrixObject *self, PyObject *args, PyObject *kwds) {
    Matrix_clear((PyObject*) self);
    static char *kwlist[] = {"data", NULL};
    PyObject *flattened_data = NULL;

    // Parse args
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &flattened_data)) return -1;

    if (!PyList_Check(flattened_data)) {
        PyErr_Format(PyExc_TypeError, "cannot construct data from object of type '%s'", Py_TYPE(flattened_data)->tp_name);
        return -1;
    }

    self->rows = PyList_Size(flattened_data);
    if (self->rows < 1) {
        PyErr_Format(PyExc_ValueError, "data cannot have %ld rows", self->rows);
        return -1;
    }

    PyObject *row_object = PyList_GetItem(flattened_data, 0);
    if (!PyList_Check(row_object)) {
        PyErr_Format(PyExc_TypeError, "data must contain only list references, but received '%s' reference ", 
            Py_TYPE(row_object)->tp_name);
        return -1;
    }

    self->cols = PyList_Size(row_object);
    if (self->cols < 1) {
        PyErr_Format(PyExc_ValueError, "data cannot have %ld cols", self->cols);
        return -1;
    }

    self->size = self->rows * self->cols;
    if (!mallocMatrixData(self)) return -1;

    for (long row_i = 0; row_i < self->rows; ++row_i) {
        row_object = PyList_GetItem(flattened_data, row_i);
        if (!PyList_Check(row_object)) {
            PyErr_Format(PyExc_TypeError, "data must contain only list references, but received '%s' reference ", 
                Py_TYPE(row_object)->tp_name);
            return -1;
        }
        long row_size = PyList_Size(row_object);
        if (row_size != self->cols) {
            PyErr_Format(PyExc_ValueError, "expected %d element(s) in row, but received %d element(s)", self->cols, row_size);
            return -1;
        }
        for (long col_i = 0; col_i < self->cols; ++col_i) {
            PyObject *item = PyList_GetItem(row_object, col_i);
            if (!PyLong_Check(item)) {
                PyErr_Format(PyExc_TypeError, "encounted non-numeric type in data: %s", Py_TYPE(item)->tp_name);
                return -1;
            }
            self->data[row_i * self->cols + col_i] = PyFloat_AsDouble(item);
        }
    }
    Py_INCREF(self);
    return 0;
}

static void Matrix_dealloc(MatrixObject *self) {
    PyObject_GC_UnTrack(self);
    Matrix_clear((PyObject*) self);
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

static void Matrix_clear(PyObject* o) {
    MatrixObject *mat = (MatrixObject*) o;
    if (mat->data) free(mat->data);
    Py_CLEAR(mat->transpose);
    Py_CLEAR(mat->Py_rows);
    Py_CLEAR(mat->Py_cols);
    Py_CLEAR(mat->Py_data);
    Py_CLEAR(mat->Py_repr);
}

static int Matrix_traverse(MatrixObject *self, visitproc visit, void *arg) {
    Py_VISIT(self->transpose);
    return 0;
}

static PyTypeObject MatrixType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "Matrix",
    .tp_doc = "Matrix class implemented in C",
    .tp_basicsize = sizeof(MatrixObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
    .tp_new = Matrix_new,
    .tp_init = (initproc) Matrix_init,
    .tp_dealloc = (destructor) Matrix_dealloc,
    .tp_methods = MatrixMethodsDefs,
    .tp_repr = (reprfunc) Matrix_repr,
    .tp_getset = MatrixGetSetDefs,
    .tp_as_number = &MatrixNumberMethods,
    .tp_clear = (inquiry) Matrix_clear,
    .tp_traverse = (traverseproc) Matrix_traverse,
};


/* PyNumberMethods functions */
static PyObject* MatrixNumber_merge(MatrixObject *mat1, MatrixObject *mat2, compfunc compress) {
    if (mat1->rows != mat2->rows || mat1->cols != mat2->cols) {
        PyErr_SetString(PyExc_ValueError, "matrices are not the same shape");
        return NULL;
    }
    MatrixObject *res = freshMatrix(mat1->rows, mat1->cols);
    if (!res) return NULL;
    if (!mallocMatrixData(res)) return NULL;
    if (!compress(mat1->data, mat2->data, res->data, mat1->size)) return NULL;
    return (PyObject*) res;
}

static int MatrixNumber_merge_add(T *a, T *b, T *res, const Py_ssize_t size) {
    for (Py_ssize_t i = 0; i < size; ++i) {
        *res = *a + *b;
        ++a;
        ++b;
        ++res;
    }
    return 1;
}

static int MatrixNumber_merge_subtract(T *a, T *b, T *res, const Py_ssize_t size) {
    for (Py_ssize_t i = 0; i < size; ++i) {
        *res = *a - *b;
        ++a;
        ++b;
        ++res;
    }
    return 1;
}

static int MatrixNumber_merge_multiply(T *a, T *b, T *res, const Py_ssize_t size) {
    for (Py_ssize_t i = 0; i < size; ++i) {
        *res = *a * *b;
        ++a;
        ++b;
        ++res;
    }
    return 1;
}

static int MatrixNumber_merge_divide(T *a, T *b, T *res, const Py_ssize_t size) {
    for (Py_ssize_t i = 0; i < size; ++i) {
        if (*b == 0) {
            PyErr_SetString(PyExc_ZeroDivisionError, "cannot divide by matrix containing a zero value");
            return 0;
        }
        *res = *a / *b;
        ++a;
        ++b;
        ++res;
    }
    return 1;
}

static PyObject* MatrixNumber_scalar_multiply(MatrixObject *mat, PyObject *scalar) {
    double k = PyFloat_AsDouble(scalar);
    MatrixObject *res = freshMatrix(mat->rows, mat->cols);
    if (!res) return NULL;
    for (Py_ssize_t i = 0; i < mat->size; ++i) {
        res->data[i] = mat->data[i] * k;
    }
    return (PyObject*) res;
}

static PyObject* MatrixNumber_scalar_divide(MatrixObject *mat, PyObject *scalar) {
    double k = PyFloat_AsDouble(scalar);
    if (k == 0) {
        PyErr_SetString(PyExc_ZeroDivisionError, "cannot divide matrix values by zero");
        return NULL;
    }
    MatrixObject *res = freshMatrix(mat->rows, mat->cols);
    if (!res) return NULL;
    for (Py_ssize_t i = 0; i < mat->size; ++i) {
        res->data[i] = mat->data[i] / k;
    }
    return (PyObject*) res;
}

static PyObject* MatrixNumber_add(PyObject *a, PyObject *b) {
    PyObject *res = NULL;
    if (QMatrix_Check(a) && QMatrix_Check(b)) {
        return MatrixNumber_merge((MatrixObject*) a, (MatrixObject*) b, MatrixNumber_merge_add);
    }
    if (!res) return unsupportedOperation(a, b, '+');
    Py_INCREF(res);
    return res;
}

static PyObject* MatrixNumber_subtract(PyObject *a, PyObject *b) {
    PyObject *res = NULL;
    if (QMatrix_Check(a) && QMatrix_Check(b)) {
        return MatrixNumber_merge((MatrixObject*) a, (MatrixObject*) b, MatrixNumber_merge_subtract);
    }
    if (!res) return unsupportedOperation(a, b, '-');
    Py_INCREF(res);
    return res;
}

static PyObject* MatrixNumber_multiply(PyObject *a, PyObject *b) {
    PyObject *res = NULL;
    if (QMatrix_Check(a) && QMatrix_Check(b)) {
        res = MatrixNumber_merge((MatrixObject*) a, (MatrixObject*) b, MatrixNumber_merge_multiply);
    }
    if (QMatrix_Check(a) && (PyLong_Check(b) || PyFloat_Check(b))) {
        res = MatrixNumber_scalar_multiply((MatrixObject*) a, b);
    }
    if (QMatrix_Check(b) && (PyLong_Check(a) || PyFloat_Check(a))) {
        res = MatrixNumber_scalar_multiply((MatrixObject*) b, a);
    }
    if (!res) return unsupportedOperation(a, b, '*');
    Py_INCREF(res);
    return res;
}

static PyObject* MatrixNumber_divide(PyObject *a, PyObject *b) {
    PyObject *res = NULL;
    if (QMatrix_Check(a) && QMatrix_Check(b)) {
        res = MatrixNumber_merge((MatrixObject*) a, (MatrixObject*) b, MatrixNumber_merge_divide);
    }
    if (QMatrix_Check(a) && (PyLong_Check(b) || PyFloat_Check(b))) {
        res = MatrixNumber_scalar_divide((MatrixObject*) a, b);
    }
    if (!res) return unsupportedOperation(a, b, '/');
    Py_INCREF(res);
    return res;
}

static PyObject* MatrixNumber_matrix_multiply(PyObject *a, PyObject *b) {
    if (!QMatrix_Check(a) || !QMatrix_Check(b)) {
        return unsupportedOperation(a, b, '@');
    }
    MatrixObject *mat1 = (MatrixObject*) a;
    MatrixObject *mat2 = (MatrixObject*) b;

    if (mat1->cols != mat2->rows) {
        PyErr_Format(PyExc_ValueError, "Matrix 1 has dims (%ld, %ld) while Matrix 2 has dims (%ld, %ld); Incompatible for multiplication", 
            mat1->rows, mat1->cols, mat2->rows, mat2->cols);
        return NULL;
    }
    MatrixObject *res = freshMatrix(mat1->rows, mat2->cols);
    if (!res) return NULL;

    T *mat1_ptr = mat1->data;
    T *mat2_ptr = mat2->data;
    T *dot_ptr = res->data;
    for (long row_i = 0; row_i < res->rows; ++row_i) {
        long col_i = 0;
        while (1) {
            *dot_ptr = 0;
            for (long i = 0; i < mat1->cols; ++i) {
                *dot_ptr += *mat1_ptr * *mat2_ptr;
                ++mat1_ptr;
                mat2_ptr += mat2->cols;
            }
            ++dot_ptr;
            ++col_i;
            mat2_ptr -= mat2->size - 1;
            if (col_i == res->cols) break;
            // only rewind row when not at end of res cols
            mat1_ptr -= mat1->cols;
        }
        mat2_ptr = mat2->data;
    }
    Py_INCREF(res);
    return (PyObject*) res;
}

static PyNumberMethods MatrixNumberMethods = {
    .nb_add = (binaryfunc) MatrixNumber_add,
    .nb_subtract = (binaryfunc) MatrixNumber_subtract,
    .nb_multiply = (binaryfunc) MatrixNumber_multiply,
    .nb_true_divide = (binaryfunc) MatrixNumber_divide,
    .nb_matrix_multiply = (binaryfunc) MatrixNumber_matrix_multiply,
};


/* QuantumMethodDef functions */
static PyObject* Quantum_relu_forwards(PyObject *self, PyObject *const *objs, Py_ssize_t nargs) {
    return activation(objs[0], relu_forwards);
}

static PyObject* Quantum_relu_backwards(PyObject *self, PyObject *const *objs, Py_ssize_t nargs) {
    return activation(objs[0], relu_backwards);
}

static PyObject* Quantum_leakyrelu_forwards(PyObject *self, PyObject *const *objs, Py_ssize_t nargs) {
    return activation(objs[0], leakyrelu_forwards);
}

static PyObject* Quantum_leakyrelu_backwards(PyObject *self, PyObject *const *objs, Py_ssize_t nargs) {
    return activation(objs[0], leakyrelu_backwards);
}

static PyObject* Quantum_tanh_forwards(PyObject *self, PyObject *const *objs, Py_ssize_t nargs) {
    return activation(objs[0], tanh_forwards);
}

static PyObject* Quantum_tanh_backwards(PyObject *self, PyObject *const *objs, Py_ssize_t nargs) {
    return activation(objs[0], tanh_backwards);
}

static PyMethodDef QuantumMethodDefs[] = {
    {"relu_forwards", (_PyCFunctionFast) Quantum_relu_forwards, METH_FASTCALL, 
     "Activate a matrix with ReLU"},
    {"relu_backwards", (_PyCFunctionFast) Quantum_relu_backwards, METH_FASTCALL, 
     "Deactivate a matrix with ReLU"},
    {"leakyrelu_forwards", (_PyCFunctionFast) Quantum_leakyrelu_forwards, METH_FASTCALL, 
     "Activate a matrix with Leaky ReLU"},
    {"leakyrelu_backwards", (_PyCFunctionFast) Quantum_leakyrelu_backwards, METH_FASTCALL, 
     "Deactivate a matrix with Leaky ReLU"},
    {"tanh_forwards", (_PyCFunctionFast) Quantum_tanh_forwards, METH_FASTCALL, 
     "Activate aith Tanh"},
    {"tanh_backwards", (_PyCFunctionFast) Quantum_tanh_backwards, METH_FASTCALL, 
     "Activate aith Tanh"},
    {NULL, NULL, 0, NULL}
};

static PyObject* activation(PyObject *o, void(*map)(T*,T*)) {
    if (!QMatrix_Check(o)) {
        PyErr_Format(PyExc_TypeError, "expected '%s' object, but received '%s' object", MatrixType.tp_name, Py_TYPE(o)->tp_name);
        return NULL;
    }
    MatrixObject *mat = (MatrixObject*) o;
    MatrixObject *res = freshMatrix(mat->rows, mat->cols);
    if (!res) return NULL;
    T *res_ptr = res->data;
    T *mat_ptr = mat->data;
    for (long i = 0; i < res->size; ++i) {
        map(mat_ptr, res_ptr);
        ++res_ptr;
        ++mat_ptr;
    }
    Py_INCREF(res);
    return (PyObject*) res;
}

static inline void relu_forwards(T *x, T *y) {
    *y = *x <= 0 ? 0 : *x;
}

static inline void relu_backwards(T *dy, T *dx) {
    *dx = *dy <= 0 ? 0 : 1;
}

static inline void leakyrelu_forwards(T *x, T *y) {
    *y = *x <= 0 ? *x * 0.01 : *x;
}

static inline void leakyrelu_backwards(T *dy, T *dx) {
    *dx = *dy <= 0 ? 0.01 : 1;
}

static inline void tanh_forwards(T *x, T *y) {
    *y = tanh(*x);
}

static inline void tanh_backwards(T *dy, T *dx) {
    // 1 - tanh^2(x)
    double tanhdy = tanh(*dy);
    *dx = 1.0 - (tanhdy * tanhdy);
}

/* Module definition with methods */
static PyModuleDef QuantumModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "Quantum Module",
    .m_doc = "#4145",
    .m_size = -1,
    .m_methods = QuantumMethodDefs,
};


PyMODINIT_FUNC PyInit_Quantum() {
    // Initialize random generator with time-based seed
    RandomGenerator_init(&rg);

    // Ready MatrixType
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
