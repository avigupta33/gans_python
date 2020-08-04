#ifndef MATRIX_H
#define MATRIX_H

#include "quantum.h"
#include "random.h"

#define Matrix_Check(o) (Py_TYPE(o) == &MatrixType)

typedef struct MatrixObject MatrixObject;

typedef bool(*mergefunc)(MatrixObject*, MatrixObject*, MatrixObject*);
typedef size_t(*idxfunc)(MatrixObject*, size_t);

struct MatrixObject {
    PyObject_HEAD
    // C data
    Py_ssize_t rows;
    Py_ssize_t cols;
    Py_ssize_t size;
    dtype *data;

    // Python data
    PyObject *transpose;
    PyObject *py_rows;
    PyObject *py_cols;
    PyObject *py_data;
    PyObject *py_repr;
    PyObject *py_str;

    // Iter data
    idxfunc row_idx;
    idxfunc col_idx;
};

static PyMethodDef MatrixMethodDefs[];
static PyGetSetDef MatrixGetSetDefs[];
static PyNumberMethods MatrixNumberMethods;
static PyTypeObject MatrixType;

// static methods
static PyObject* matrix_class_gauss(PyObject *o, PyObject *args, PyObject *kwds); // create gauss-distributed matrix, sets error flag and returns NULL on failure
// mu should default to 0, sigma defaults to 1
static PyObject* matrix_class_uniform(PyObject *o, PyObject *args, PyObject *kwds); // create uniformly-distributed matrix, sets error flag and returns NULL on failure
static PyObject* matrix_map(PyObject *o, PyObject *func); // apply an element-wise mapping function

// static methods (internal)
static bool _matrix_parse_dims(PyObject *py_dims, Py_ssize_t *dims); // parse dims tuple, sets error flag and returns false on failure

// matrix type methods
static PyObject* matrix_repr(MatrixObject *self);
static PyObject* matrix_str(MatrixObject *self);
static PyObject* matrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static PyObject* matrix_richcompare(MatrixObject *self, PyObject *obj, int op);
static int matrix_init(MatrixObject *self, PyObject *args, PyObject *kwds);
static int matrix_traverse(MatrixObject *self, visitproc visit, void *arg);
static void matrix_clear(MatrixObject *self);
static void matrix_dealloc(MatrixObject *self);

// matrix type methods (internal)
static int _matrix_init_copy_matrix(MatrixObject *self, PyObject *py_matrix);
static int _matrix_init_from_scalar(MatrixObject *self, PyObject *py_scalar, PyObject *py_dims);
static int _matrix_init_from_1d_list(MatrixObject *self, PyObject *py_vector, PyObject *py_dims);
static int _matrix_init_from_2d_list(MatrixObject *self, PyObject *py_matrix, PyObject *py_dims);
static bool _matrix_alloc_data(MatrixObject *self); // alloc matrix data, sets error flag and returns false on failure
static bool _matrix_unbind_state_objects(MatrixObject *self, bool keep_data); // unbinds PyObject's related to state, loses data if keep_data == false
static bool _matrix_not_equal(MatrixObject *mat1, MatrixObject *mat2);
static void _matrix_destroy(MatrixObject *self); // frees self
static PyObject* _matrix_create_from_dims(Py_ssize_t rows, Py_ssize_t cols); // create matrix with allocd data, sets error flag and returns NULL on failure
static PyObject* _matrix_bad_init(MatrixObject *self); // frees self, sets error flag and returns NULL
static PyObject* _matrix_number_not_implemented(PyObject *obj1, PyObject *obj2, char *op_id);

// matrix get-set methods
static PyObject* matrix_get_rows(MatrixObject *self);
static PyObject* matrix_get_cols(MatrixObject *self);
static PyObject* matrix_get_data(MatrixObject *self);
static PyObject* matrix_get_transpose(MatrixObject *self);

// matrix number methods TODO: implement
static PyObject* matrix_number_add(PyObject *obj1, PyObject *obj2);
static PyObject* matrix_number_sub(PyObject *obj1, PyObject *obj2);
static PyObject* matrix_number_mul(PyObject *obj1, PyObject *obj2);
// static PyObject* matrix_number_pow(PyObject *obj1, PyObject *obj2, PyObject *obj3); // element-wise power
// static PyObject* matrix_number_negative(PyObject *obj1);
// static PyObject* matrix_number_positive(PyObject *obj1);
// static PyObject* matrix_number_absolute(PyObject *obj1);
// static PyObject* matrix_number_inplace_add(PyObject *obj1, PyObject *obj2);
// static PyObject* matrix_number_inplace_sub(PyObject *obj1, PyObject *obj2);
// static PyObject* matrix_number_inplace_mul(PyObject *obj1, PyObject *obj2);
// static PyObject* matrix_number_inplace_pow(PyObject *obj1, PyObject *obj2, PyObject *obj3); // element-wise power
static PyObject* matrix_number_div(PyObject *obj1, PyObject *obj2);
// static PyObject* matrix_number_inplace_div(PyObject *obj1, PyObject *obj2);
// static PyObject* matrix_number_index(PyObject *obj1);
static PyObject* matrix_number_matmul(PyObject *obj1, PyObject *obj2);

// matrix number methods (internal)
static PyObject* _matrix_number_merge(MatrixObject *mat1, MatrixObject *mat2, mergefunc merge);
static PyObject* _matrix_number_scalar_mul(MatrixObject *mat, double scalar);
static PyObject* _matrix_number_scalar_div(MatrixObject *mat, double scalar);
static bool _matrix_number_merge_add(MatrixObject *mat1, MatrixObject *mat2, MatrixObject *mat3);
static bool _matrix_number_merge_sub(MatrixObject *mat1, MatrixObject *mat2, MatrixObject *mat3);
static bool _matrix_number_merge_mul(MatrixObject *mat1, MatrixObject *mat2, MatrixObject *mat3);
static bool _matrix_number_merge_div(MatrixObject *mat1, MatrixObject *mat2, MatrixObject *mat3);
static bool _matrix_number_scalar_check(PyObject *scalar); // check if scalar is convertible to a C double
static double _matrix_number_scalar_as_double(PyObject *number); // convert scalar to a C double

// matrix indexing methods (internal)
static size_t _matrix_idx_row_major(MatrixObject *mat, size_t i); // iter for row-major matrices
static size_t _matrix_idx_col_major(MatrixObject *mat, size_t i); // iter for col-major matrices

#endif /* MATRIX_H */