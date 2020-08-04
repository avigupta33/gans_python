#include "../include/matrix.h"

static PyMethodDef MatrixMethodDefs[] = {
    {"gauss", (PyCFunction) matrix_class_gauss, METH_CLASS | METH_VARARGS | METH_KEYWORDS,
     "Construct a matrix initialized from a Gaussian distribution"},
    {"uniform", (PyCFunction) matrix_class_uniform, METH_CLASS | METH_VARARGS | METH_KEYWORDS,
     "Construct a matrix initialized from a uniform distribution"},
    {"map", (PyCFunction) matrix_map, METH_O,
     "Apply an element-wise mapping function"},
    {NULL, NULL, 0, NULL}
};

static PyGetSetDef MatrixGetSetDefs[] = {
    {"rows", (getter) matrix_get_rows, NULL,
     "Get rows"},
    {"cols", (getter) matrix_get_cols, NULL,
     "Get cols"},
    {"data", (getter) matrix_get_data, NULL,
     "Get data"},
    {"T", (getter) matrix_get_transpose, NULL,
     "Get transpose"},
    {NULL, NULL, NULL, NULL}
};

static PyNumberMethods MatrixNumberMethods = {
    .nb_add = (binaryfunc) matrix_number_add,
    .nb_subtract = (binaryfunc) matrix_number_sub,
    .nb_multiply = (binaryfunc) matrix_number_mul,
    // .nb_power = (ternaryfunc) matrix_number_pow,
    // .nb_negative = (unaryfunc) matrix_number_negative,
    // .nb_positive = (unaryfunc) matrix_number_positive,
    // .nb_absolute = (unaryfunc) matrix_number_absolute,
    // .nb_inplace_add = (binaryfunc) matrix_number_inplace_add,
    // .nb_inplace_subtract = (binaryfunc) matrix_number_inplace_sub,
    // .nb_inplace_multiply = (binaryfunc) matrix_number_inplace_mul,
    // .nb_inplace_power = (ternaryfunc) matrix_number_inplace_pow,
    .nb_true_divide = (binaryfunc) matrix_number_div,
    // .nb_inplace_true_divide = (binaryfunc) matrix_number_inplace_div,
    // .nb_index = (unaryfunc) matrix_number_index,
    .nb_matrix_multiply = (binaryfunc) matrix_number_matmul,
};

static PyTypeObject MatrixType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "Matrix",
    .tp_doc = "Matrix class",
    .tp_basicsize = sizeof(MatrixObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
    .tp_new = matrix_new,
    .tp_init = (initproc) matrix_init,
    .tp_dealloc = (destructor) matrix_dealloc,
    .tp_methods = MatrixMethodDefs,
    .tp_repr = (reprfunc) matrix_repr,
    .tp_str = (reprfunc) matrix_str,
    .tp_getset = MatrixGetSetDefs,
    .tp_as_number = &MatrixNumberMethods,
    .tp_traverse = (traverseproc) matrix_traverse,
    .tp_clear = (inquiry) matrix_clear,
    .tp_richcompare = (richcmpfunc) matrix_richcompare,
};

// static methods
static PyObject* matrix_class_gauss(PyObject *o, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"mu", "sigma", "dims", "seed", NULL};
    double mu = 0;
    double sigma = 1;
    PyObject *py_dims = NULL;
    unsigned int seed = time(NULL);
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|dd$OI", kwlist, 
        &mu,
        &sigma,
        &py_dims,
        &seed)) return NULL;

    Py_ssize_t dims[2] = {1, 1};
    if (py_dims && !_matrix_parse_dims(py_dims, dims)) return NULL;

    MatrixObject *self = (MatrixObject*) _matrix_create_from_dims(dims[0], dims[1]);
    if (!self) return NULL;

    set_seed(seed);
    for (it i = 0; i < self->size; ++i) {
        self->data[i] = gauss(mu, sigma);
    }

    return (PyObject*) self;
}

static PyObject* matrix_class_uniform(PyObject *o, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"lower", "upper", "dims", "seed", NULL};
    double lower = 0;
    double upper = 1;
    PyObject *py_dims = NULL;
    unsigned int seed = time(NULL);
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|dd$OI", kwlist, 
        &lower,
        &upper,
        &py_dims,
        &seed)) return NULL;

    if (lower > upper) {
        char precision[] = "%.3f";
        char lower_str[snprintf(NULL, 0, precision, lower)];
        char upper_str[snprintf(NULL, 0, precision, upper)];
        sprintf(lower_str, precision, lower);
        sprintf(upper_str, precision, upper);
        return PyErr_Format(
            PyExc_ValueError,
            "lower bound cannot be greater than upper bound: (%s, %s)",
            lower_str,
            upper_str);
    }

    Py_ssize_t dims[2] = {1, 1};
    if (py_dims && !_matrix_parse_dims(py_dims, dims)) return NULL;

    MatrixObject *self = (MatrixObject*) _matrix_create_from_dims(dims[0], dims[1]);
    if (!self) return NULL;

    set_seed(seed);
    for (it i = 0; i < self->size; ++i) {
        self->data[i] = uniform(lower, upper);
    }

    return (PyObject*) self;
}

static PyObject* matrix_map(PyObject *o, PyObject *func) {
    if (!PyCallable_Check(func)) {
        return _quantum_type_error(
            "mapping",
            &PyFunction_Type,
            Py_TYPE(func));
    }

    MatrixObject *self = (MatrixObject*) o;
    PyObject *py_data = matrix_get_data(self); // py_data is INCREF'd for caller (this function) automatically
    if (!_matrix_unbind_state_objects(self, true)) return NULL; // without INCREF, py_data could be freed here
    for (it i = 0; i < self->size; ++i) {
        // in py_data, it's always in row_major order compared to self
        PyObject *item = PyTuple_GET_ITEM(py_data, i);
        PyObject *mapped_item = PyObject_CallFunctionObjArgs(func, item, NULL);
        if (!mapped_item) {
            // TODO: How to add the error to the end of my exception?
            dtype element = self->data[self->row_idx(self, i)];
            char item_str[snprintf(NULL, 0, "%.4f", element)];
            sprintf(item_str, "%.4f", element);

            const char *callable_str = PyUnicode_AsUTF8(
                PyFunction_Check(func)
                ? ((PyFunctionObject*) func)->func_name 
                : (PyObject_Repr(func)));

            return PyErr_Format(
                PyExc_ValueError,
                "Mapping '%s' raised exception when called with '%s'",
                callable_str,
                item_str);
        }
        if (!_matrix_number_scalar_check(mapped_item)) {
            return _quantum_type_error(
                "mapped element",
                &PyFloat_Type,
                Py_TYPE(mapped_item));
        }
        // in data, it's not necessarily row major, so we have to use its row_idx function
        self->data[self->row_idx(self, i)] = _matrix_number_scalar_as_double(mapped_item);
    }
    Py_DECREF(py_data); // this function no longer needs py_data, so DECREF
    Py_INCREF(self); // the caller is now referencing self
    return (PyObject*) self;
}

// static methods (internal)
static bool _matrix_parse_dims(PyObject *py_dims, Py_ssize_t *dims) {
    dims[1] = 1;

    if (!PyTuple_Check(py_dims)) {
        _quantum_type_error(
            "dims",
            &PyTuple_Type,
            Py_TYPE(py_dims));
        return false;
    }

    Py_ssize_t dims_received = PyTuple_Size(py_dims);
    if (dims_received > 2) {
        PyErr_Format(
            PyExc_ValueError,
            "dims cannot have more than 2 args, found %ld args",
            dims_received);
        return false;
    }

    for (it i = 0; i < dims_received; ++i) {
        PyObject *item = PyTuple_GetItem(py_dims, i);
        if (!PyLong_Check(item)) {
            _quantum_type_error(
                "dims",
                &PyLong_Type,
                Py_TYPE(item));
            return false;
        }
        dims[i] = PyLong_AsLong(item);
    }

    if (dims[0] < 1 || dims[1] < 1) {
        PyErr_Format(
            PyExc_ValueError,
            "dims must be positive, non-zero values: (%ld, %ld)",
            dims[0],
            dims[1]);
        return false;
    }
    return true;
}

// matrix type methods
static PyObject* matrix_repr(MatrixObject *self) {
    if (!self->py_repr) {
        // calculate size of repr buffer
        char repr_dims[] = "rows = %ld\ncols = %ld\n";
        char repr_data_head[] = "data = [";
        char repr_data_tail[] = "]";
        char delim[] = ", ";
        char precision[] = "%.4f";

        size_t repr_size = 1; // for null-terminating char
        repr_size += snprintf(NULL, 0, repr_dims, self->rows, self->cols);
        repr_size += snprintf(NULL, 0, repr_data_head, NULL);
        repr_size += snprintf(NULL, 0, repr_data_tail, NULL);
        repr_size += snprintf(NULL, 0, delim, NULL) * (self->size - 1);
        for (it i = 0; i < self->size; ++i) {
            repr_size += snprintf(NULL, 0, precision, self->data[self->row_idx(self, i)]);
        }
        char repr[repr_size];

        // populate repr buffer
        it repr_i = 0;
        repr_i += sprintf(&repr[repr_i], repr_dims, self->rows, self->cols);
        repr_i += sprintf(&repr[repr_i], repr_data_head, NULL);

        for (it i = 0; true;) {
            repr_i += sprintf(&repr[repr_i], precision, self->data[self->row_idx(self, i)]);
            if (++i == self->size) break;
            repr_i += sprintf(&repr[repr_i], delim, NULL);
        }
        repr_i += sprintf(&repr[repr_i], repr_data_tail, NULL);

        self->py_repr = PyUnicode_FromString(repr);

        if (!self->py_repr) return PyErr_NoMemory();
    }
    Py_INCREF(self->py_repr);
    return self->py_repr;
}

static PyObject* matrix_str(MatrixObject *self) {
    if (!self->py_str) {
        // calculate size of str buffer
        char str_edge_head[] = "[ ";
        char str_edge_tail[] = " ]";
        char delim[] = " ";

        size_t str_size = 1; // for null-terminating char
        str_size += snprintf(NULL, 0, delim, NULL) * (self->size - self->rows);
        str_size += snprintf(NULL, 0, str_edge_head, NULL) * self->rows;
        str_size += snprintf(NULL, 0, str_edge_tail, NULL) * self->rows;
        str_size += self->rows - 1; // '\n' at the end of each row except last
        size_t val_size = 7; // all values will be 7 chars, right-aligned
        str_size += val_size * self->size;
        char str[str_size];

        // populate str buffer
        it str_i = 0;
        it data_i = 0;
        for (it row_i = 0; true;) {
            str_i += sprintf(&str[str_i], str_edge_head, NULL);
            for (it col_i = 0; true;) {
                dtype val = self->data[self->row_idx(self, data_i++)];
                str_i += sprintf(&str[str_i], (val >= 1000 ? "%7.1e" : "%7.3f"), val);
                if (++col_i == self->cols) break;
                str_i += sprintf(&str[str_i], delim, NULL);
            }
            str_i += sprintf(&str[str_i], str_edge_tail, NULL);
            if (++row_i == self->rows) break;
            str_i += sprintf(&str[str_i], "\n");
        }

        self->py_str = PyUnicode_FromString(str);

        if (!self->py_str) return PyErr_NoMemory();
    }
    Py_INCREF(self->py_str);
    return self->py_str;
}

static PyObject* matrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    MatrixObject *self = (MatrixObject*) type->tp_alloc(type, 0);
    if (!self) return _matrix_bad_init(self);
    self->rows = 0;
    self->cols = 0;
    self->size = 0;
    self->data = NULL;

    self->transpose = NULL;
    self->py_rows = NULL;
    self->py_cols = NULL;
    self->py_data = NULL;
    self->py_repr = NULL;
    self->py_str = NULL;

    self->row_idx = _matrix_idx_row_major;
    self->col_idx = _matrix_idx_col_major;
    return (PyObject*) self;
}

static PyObject* matrix_richcompare(MatrixObject *self, PyObject *obj, int op) {
    char *op_id = "";
    if (Matrix_Check(obj)) {
        MatrixObject *other = (MatrixObject*) obj;
        PyObject *ret = NULL;
        switch (op) {
            case Py_EQ : {
                ret = !_matrix_not_equal(self, other) 
                    ? Py_True 
                    : Py_False;
                break;
            }
            case Py_NE : {
                ret = _matrix_not_equal(self, other)
                    ? Py_True
                    : Py_False;
                break;
            }
            case Py_LT : { op_id = "<"; break; }
            case Py_LE : { op_id = "<="; break; }
            case Py_GT : { op_id = ">"; break; }
            case Py_GE : { op_id = ">="; break; }
        }
        if (ret) {
            Py_INCREF(ret);
            return ret;
        }
    }
    return _matrix_number_not_implemented(
        (PyObject*) self,
        obj,
        op_id);
}

static int matrix_init(MatrixObject *self, PyObject *args, PyObject *kwds) {
    matrix_clear(self);
    static char *kwlist[] = {"data", "dims", NULL};
    PyObject *py_data = NULL;
    PyObject *py_dims = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O|$O", kwlist, 
        &py_data, 
        &py_dims)) return -1;

    if (Matrix_Check(py_data)) {
        return _matrix_init_copy_matrix(self, py_data);
    } else if (_matrix_number_scalar_check(py_data)) {
        return _matrix_init_from_scalar(self, py_data, py_dims);
    } else if (PyList_Check(py_data)) {
        if (Py_SIZE(py_data) < 1) {
            PyErr_Format(
                PyExc_ValueError,
                "matrix constructed from list must have at least one element");
            return -1;
        }

        PyObject *first = PyList_GET_ITEM(py_data, 0);
        if (_matrix_number_scalar_check(first)) {
            return _matrix_init_from_1d_list(self, py_data, py_dims);
        } else if (PyList_Check(first)) {
            return _matrix_init_from_2d_list(self, py_data, py_dims);
        } else {
            PyErr_Format(
                PyExc_NotImplementedError,
                "matrix cannot be constructed from data of type 'List[%s]'",
                Py_TYPE(py_data)->tp_name);
            return -1;
        }
    } else {
        PyErr_Format(
            PyExc_NotImplementedError,
            "matrix cannot be constructed from data of type '%s'",
            Py_TYPE(py_data)->tp_name);
        return -1;
    }

    return 0;
}

static int matrix_traverse(MatrixObject *self, visitproc visit, void *arg) {
    Py_VISIT(self->transpose);
    return 0;
}

static void matrix_clear(MatrixObject *self) {
    if (self) {
        if (self->data) {
            if (self->transpose) {
                _matrix_unbind_state_objects(self, false);
            } else {
                free(self->data);
            }
        }
        Py_CLEAR(self->py_rows);
        Py_CLEAR(self->py_cols);
;
    }
}

static void matrix_dealloc(MatrixObject *self) {
    PyObject_GC_UnTrack(self);
    _matrix_destroy(self);
}

// matrix type methods (internal)
static int _matrix_init_copy_matrix(MatrixObject *self, PyObject *py_matrix) {
    MatrixObject *src = (MatrixObject*) py_matrix;
    self->rows = src->rows;
    self->cols = src->cols;
    self->size = src->size;
    if (!_matrix_alloc_data(self)) return -1;
    memcpy(self->data, src->data, sizeof(dtype) * self->size);
    self->row_idx = src->row_idx;
    self->col_idx = src->col_idx;
    return 0;
}

static int _matrix_init_from_scalar(MatrixObject *self, PyObject *py_scalar, PyObject *py_dims) {
    Py_ssize_t dims[2] = {1, 1};
    if (py_dims && !_matrix_parse_dims(py_dims, dims)) return -1;

    dtype scalar = _matrix_number_scalar_as_double(py_scalar);
    self->rows = dims[0];
    self->cols = dims[1];
    self->size = dims[0] * dims[1];
    if (!_matrix_alloc_data(self)) return -1;

    for (it i = 0; i < self->size; ++i) {
        self->data[i] = scalar;
    }

    return 0;
}

static int _matrix_init_from_1d_list(MatrixObject *self, PyObject *py_vector, PyObject *py_dims) {
    Py_ssize_t dims[2] = {Py_SIZE(py_vector), 1};
    if (py_dims && !_matrix_parse_dims(py_dims, dims)) return -1;
    if (dims[0] * dims[1] != Py_SIZE(py_vector)) {
        PyErr_Format(
            PyExc_ValueError,
            "matrix with dims=(%ld, %ld) cannot be created from %d value(s)",
            dims[0],
            dims[1],
            Py_SIZE(py_vector));
        return -1;
    }
    self->rows = dims[0];
    self->cols = dims[1];
    self->size = dims[0] * dims[1];

    if (!_matrix_alloc_data(self)) return -1;

    for (it i = 0; i < self->size; ++i) {
        PyObject *item = PyList_GET_ITEM(py_vector, i);
        if (!_matrix_number_scalar_check(item)) {
            _quantum_type_error(
                "matrix elements",
                &PyFloat_Type,
                Py_TYPE(item));
            return -1;
        }
        self->data[i] = _matrix_number_scalar_as_double(item);
    }
    return 0;
}

static int _matrix_init_from_2d_list(MatrixObject *self, PyObject *py_matrix, PyObject *py_dims) {
    Py_ssize_t dims[2] = {
        Py_SIZE(py_matrix), 
        Py_SIZE(PyList_GET_ITEM(py_matrix, 0))
    };
    
    if (dims[1] < 1) {
        PyErr_Format(
            PyExc_ValueError,
            "dims must be positive, non-zero values: (%ld, %ld)",
            dims[0],
            dims[1]);
        return -1;
    }

    if (py_dims) {
        Py_ssize_t dims_received[2];
        if (!_matrix_parse_dims(py_dims, dims_received)) return -1;
        if (dims[0] != dims_received[0] || dims[1] != dims_received[1]) {
            PyErr_Format(
                PyExc_ValueError,
                "dims of given 2D array (%ld, %ld) don't match dims specified (%ld, %ld)",
                dims[0],
                dims[1],
                dims_received[0],
                dims_received[1]);
            return -1;
        }
    }

    self->rows = dims[0];
    self->cols = dims[1];
    self->size = dims[0] * dims[1];
    if (!_matrix_alloc_data(self)) return -1;

    it data_i = 0;
    for (it row_i = 0; row_i < self->rows; ++row_i) {
        PyObject *py_row = PyList_GET_ITEM(py_matrix, row_i);
        if (!PyList_Check(py_row)) {
            _quantum_type_error(
                "rows in data",
                &PyList_Type,
                Py_TYPE(py_row));
            return -1;
        }

        Py_ssize_t row_size = Py_SIZE(py_row);
        if (row_size != self->cols) {
            PyErr_Format(
                PyExc_ValueError, 
                "expected %ld element(s) in row %ld, but found %ld element(s)", 
                self->cols, 
                row_i, 
                row_size);
            return -1;
        }

        for (it col_i = 0; col_i < self->cols; ++col_i) {
            PyObject *item = PyList_GET_ITEM(py_row, col_i);
            if (!_matrix_number_scalar_check(item)) {
                _quantum_type_error(
                    "matrix data",
                    &PyFloat_Type,
                    Py_TYPE(item));
                return -1;
            }
            self->data[data_i++] = _matrix_number_scalar_as_double(item);
        }
    }
    return 0;

    // build matrix here
}

static bool _matrix_alloc_data(MatrixObject *self) {
    if (self) {
        self->data = (dtype*) malloc(sizeof(dtype) * self->size);
        if (!self->data) {
            PyErr_NoMemory();
            return false;
        }
        return true;
    }
    return false;
}

static bool _matrix_unbind_state_objects(MatrixObject *self, bool keep_data) {
    if (self && self->transpose) {
        MatrixObject *t = (MatrixObject*) self->transpose;
        self->data = NULL;

        if (keep_data) {
            if (!_matrix_alloc_data(self)) return false;
            memcpy((void*) self->data, (void*) t->data, sizeof(dtype) * self->size);
        }

        t->transpose = NULL;
        self->transpose = NULL;
        Py_DECREF(t);
        Py_DECREF(self);
    }
    Py_CLEAR(self->py_data);
    Py_CLEAR(self->py_repr);
    Py_CLEAR(self->py_str);
    return true;
}

static bool _matrix_not_equal(MatrixObject *mat1, MatrixObject *mat2) {
    if (mat1->size != mat2->size) return true;
    if (mat1->rows != mat2->rows || mat1->cols != mat2->cols) return true;

    for (it i = 0; i < mat1->size; ++i) {
        if (mat1->data[mat1->row_idx(mat1, i)] != mat2->data[mat2->row_idx(mat2, i)]) return true;
    }
    return false;
}

static void _matrix_destroy(MatrixObject *self) {
    if (self) {
        matrix_clear(self);
        Py_TYPE(self)->tp_free((PyObject*) self);
    }
}

static PyObject* _matrix_create_from_dims(Py_ssize_t rows, Py_ssize_t cols) {
    MatrixObject *self = (MatrixObject*) matrix_new(&MatrixType, NULL, NULL);
    if (!self) return NULL;
    self->rows = rows;
    self->cols = cols;
    self->size = rows * cols;
    if (!_matrix_alloc_data(self)) return NULL;
    return (PyObject*) self;
}

static PyObject* _matrix_bad_init(MatrixObject *self) {
    _matrix_destroy(self);
    return PyErr_NoMemory();
}

static PyObject* _matrix_number_not_implemented(PyObject *obj1, PyObject *obj2, char *op_id) {
    return PyErr_Format(
        PyExc_TypeError,
        "unsupported operand type(s) for '%s': '%s' and '%s'",
        op_id,
        Py_TYPE(obj1)->tp_name,
        Py_TYPE(obj2)->tp_name);
}

// matrix get-set methods
static PyObject* matrix_get_rows(MatrixObject *self) {
    if (!self->py_rows) {
        self->py_rows = PyLong_FromSsize_t(self->rows);
        if (!self->py_rows) return PyErr_NoMemory();
    }
    Py_INCREF(self->py_rows);
    return self->py_rows;
}

static PyObject* matrix_get_cols(MatrixObject *self) {
    if (!self->py_cols) {
        self->py_cols = PyLong_FromSsize_t(self->cols);
        if (!self->py_cols) return PyErr_NoMemory();
    }
    Py_INCREF(self->py_cols);
    return self->py_cols;
}

static PyObject* matrix_get_data(MatrixObject *self) {
    if (!self->py_data) {
        self->py_data = PyTuple_New(self->size);
        if (!self->py_data) return PyErr_NoMemory();

        for (it i = 0; i < self->size; ++i) {
            PyObject *item = PyFloat_FromDouble(self->data[self->row_idx(self, i)]);
            if (!item) return NULL;
            PyTuple_SET_ITEM(self->py_data, i, item);
        }
        Py_INCREF(self->py_data); // self is referencing py_data
    }
    Py_INCREF(self->py_data); // the caller is referencing py_data
    return self->py_data;
}

static PyObject* matrix_get_transpose(MatrixObject *self) {
    if (!self->transpose) {
        MatrixObject *t = (MatrixObject*) matrix_new(&MatrixType, NULL, NULL);
        t->rows = self->cols;
        t->cols = self->rows;
        t->size = self->size;
        t->data = self->data;

        t->transpose = (PyObject*) self;
        t->py_rows = self->py_cols;
        t->py_cols = self->py_rows;
        t->py_data = NULL;
        t->py_repr = NULL;
        t->py_str = NULL;

        t->row_idx = self->col_idx;
        t->col_idx = self->row_idx;

        self->transpose = (PyObject*) t;

        Py_XINCREF(t->py_rows);
        Py_XINCREF(t->py_cols);
        Py_INCREF(t); // self references t
        Py_INCREF(self); // t references self
    }
    Py_INCREF(self->transpose); // caller references self->transpose
    return self->transpose;
}

// matrix number methods
static PyObject* matrix_number_add(PyObject *obj1, PyObject *obj2) {
    return (Matrix_Check(obj1) && Matrix_Check(obj2))
        ? _matrix_number_merge((MatrixObject*) obj1,(MatrixObject*) obj2, _matrix_number_merge_add)
        : _matrix_number_not_implemented(obj1, obj2, "+");
}

static PyObject* matrix_number_sub(PyObject *obj1, PyObject *obj2) {
    return (Matrix_Check(obj1) && Matrix_Check(obj2))
        ? _matrix_number_merge((MatrixObject*) obj1, (MatrixObject*) obj2, _matrix_number_merge_sub)
        : _matrix_number_not_implemented(obj1, obj2, "-");
}

static PyObject* matrix_number_mul(PyObject *obj1, PyObject *obj2) {
    if (Matrix_Check(obj1) && Matrix_Check(obj2)) {
        return _matrix_number_merge((MatrixObject*) obj1, (MatrixObject*) obj2, _matrix_number_merge_mul);
    } else if (Matrix_Check(obj1) && _matrix_number_scalar_check(obj2)) {
        return _matrix_number_scalar_mul((MatrixObject*) obj1, _matrix_number_scalar_as_double(obj2));
    } else if (Matrix_Check(obj2) && _matrix_number_scalar_check(obj1)) {
        return _matrix_number_scalar_mul((MatrixObject*) obj2, _matrix_number_scalar_as_double(obj1));
    }
    return _matrix_number_not_implemented(obj1, obj2, "*");
}

// static PyObject* matrix_number_negative(PyObject *obj1);
// static PyObject* matrix_number_positive(PyObject *obj1);
// static PyObject* matrix_number_absolute(PyObject *obj1);
// static PyObject* matrix_number_pow(PyObject *obj1, PyObject *obj2, PyObject *obj3);
// static PyObject* matrix_number_inplace_add(PyObject *obj1, PyObject *obj2);
// static PyObject* matrix_number_inplace_sub(PyObject *obj1, PyObject *obj2);
// static PyObject* matrix_number_inplace_mul(PyObject *obj1, PyObject *obj2);
// static PyObject* matrix_number_inplace_pow(PyObject *obj1, PyObject *obj2, PyObject *obj3);

static PyObject* matrix_number_div(PyObject *obj1, PyObject *obj2) {
    if (Matrix_Check(obj1) && Matrix_Check(obj2)) {
        return _matrix_number_merge((MatrixObject*) obj1, (MatrixObject*) obj2, _matrix_number_merge_div);
    } else if (Matrix_Check(obj1) && _matrix_number_scalar_check(obj2)) {
        return _matrix_number_scalar_div((MatrixObject*) obj1, _matrix_number_scalar_as_double(obj2));
    }
    return _matrix_number_not_implemented(obj1, obj2, "/");
}

// static PyObject* matrix_number_inplace_div(PyObject *obj1, PyObject *obj2);
// static PyObject* matrix_number_index(PyObject *obj1);

static PyObject* matrix_number_matmul(PyObject *obj1, PyObject *obj2) {
    if (!Matrix_Check(obj1) || !Matrix_Check(obj2)) {
        return _matrix_number_not_implemented(obj1, obj2, "@");
    }

    MatrixObject *mat1 = (MatrixObject*) obj1;
    MatrixObject *mat2 = (MatrixObject*) obj2;

    if (mat1->cols != mat2->rows) {
        return PyErr_Format(
            PyExc_ValueError, 
            "Matrix 1 has dims (%ld, %ld) while Matrix 2 has dims (%ld, %ld); Incompatible for multiplication.", 
            mat1->rows, 
            mat1->cols, 
            mat2->rows, 
            mat2->cols);
    }

    MatrixObject *res = (MatrixObject*) _matrix_create_from_dims(mat1->rows, mat2->cols);
    if (!res) return NULL;

    // matrix multiplication algorithm
    dtype *res_ptr = res->data;
    for (it row_i = 0; row_i < res->rows; ++row_i) {
        for (it col_i = 0; col_i < res->cols; ++col_i) {
            *res_ptr = 0;
            for (it i = 0; i < mat1->cols; ++i) {
                *res_ptr += 
                    mat1->data[mat1->row_idx(mat1, row_i * mat1->cols + i)] * 
                    mat2->data[mat2->col_idx(mat2, col_i * mat2->rows + i)];
            }
            ++res_ptr;
        }
    }

    res->py_rows = mat1->py_rows;
    res->py_cols = mat2->py_cols;
    Py_XINCREF(res->py_rows);
    Py_XINCREF(res->py_cols);
    return (PyObject*) res;
}

// matrix number methods (internal)
static PyObject* _matrix_number_merge(MatrixObject *mat1, MatrixObject *mat2, mergefunc merge) {
    if (mat1->rows != mat2->rows || mat1->cols != mat2->cols) {
        return PyErr_Format(
            PyExc_ValueError, 
            "matrices are not the same shape");
    }
    MatrixObject *res = (MatrixObject*) _matrix_create_from_dims(mat1->rows, mat1->cols);
    if (!res) return NULL;
    if (!merge(mat1, mat2, res)) return NULL;
    return (PyObject*) res;
}

static PyObject* _matrix_number_scalar_mul(MatrixObject *mat, double scalar) {
    MatrixObject *res = (MatrixObject*) _matrix_create_from_dims(mat->rows, mat->cols);
    if (!res) return NULL;

    for (it i = 0; i < res->size; ++i) {
        res->data[i] = mat->data[mat->row_idx(mat, i)] * scalar;
    }
    return (PyObject*) res;
}

static PyObject* _matrix_number_scalar_div(MatrixObject *mat, double scalar) {
    if (scalar == 0) {
        return PyErr_Format(
            PyExc_ZeroDivisionError,
            "divisor scalar cannot be 0");
    }

    MatrixObject *res = (MatrixObject*) _matrix_create_from_dims(mat->rows, mat->cols);
    if (!res) return NULL;

    for (it i = 0; i < res->size; ++i) {
        res->data[i] = mat->data[mat->row_idx(mat, i)] / scalar;
    }
    return (PyObject*) res;
}

static bool _matrix_number_merge_add(MatrixObject *mat1, MatrixObject *mat2, MatrixObject *mat3) {
    for (it i = 0; i < mat1->size; ++i) {
        mat3->data[i] = 
            mat1->data[mat1->row_idx(mat1, i)] + 
            mat2->data[mat2->row_idx(mat2, i)];
    }
    return true;
}

static bool _matrix_number_merge_sub(MatrixObject *mat1, MatrixObject *mat2, MatrixObject *mat3) {
    for (it i = 0; i < mat1->size; ++i) {
        mat3->data[i] = 
            mat1->data[mat1->row_idx(mat1, i)] - 
            mat2->data[mat2->row_idx(mat2, i)];
    }
    return true;
}

static bool _matrix_number_merge_mul(MatrixObject *mat1, MatrixObject *mat2, MatrixObject *mat3) {
    for (it i = 0; i < mat1->size; ++i) {
        mat3->data[i] = 
            mat1->data[mat1->row_idx(mat1, i)] * 
            mat2->data[mat2->row_idx(mat2, i)];
    }
    return true;
}

static bool _matrix_number_merge_div(MatrixObject *mat1, MatrixObject *mat2, MatrixObject *mat3) {
    for (it i = 0; i < mat1->size; ++i) {
        dtype divisor = mat2->data[mat2->row_idx(mat2, i)];
        if (divisor == 0) {
            PyErr_Format(
                PyExc_ZeroDivisionError,
                "divisor matrix cannot contain 0");
            return false;
        }
        mat3->data[i] = mat1->data[mat1->row_idx(mat1, i)] / divisor;
    }
    return true;
}

static bool _matrix_number_scalar_check(PyObject *scalar) {
    return PyLong_Check(scalar) || PyFloat_Check(scalar);
}

static double _matrix_number_scalar_as_double(PyObject *number) {
    // PyFloat_AsDouble() will set appropriate error flag on failure
    return PyLong_Check(number) 
        ? PyLong_AsDouble(number) 
        : PyFloat_AsDouble(number);
}

// matrix indexing methods (internal)
static size_t _matrix_idx_row_major(MatrixObject *mat, size_t i) {
    return i;
}

static size_t _matrix_idx_col_major(MatrixObject *mat, size_t i) {
    return (i * mat->rows) + (i / mat->cols) * (1 - mat->size);
}
