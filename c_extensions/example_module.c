
#include <python3.7/Python.h>


int seq(int target, int *arr) {
    if (target < 1) return arr[0];
    int sum = arr[0] + arr[1];
    arr[0] = arr[1];
    arr[1] = sum;
    return seq(target-1, arr);
}

int fibC(int n) {
    int arr[] = {0,1};
    return seq(n, arr);
}

static PyObject* example_fib(PyObject *self, PyObject *args) {
    int n;
    if (!PyArg_ParseTuple(args, "i", &n)) return NULL;

    return Py_BuildValue("i", fibC(n));
}

static PyObject* example_getQuinnStr(PyObject* self) {
    return Py_BuildValue("s", "Quinn");
}


/* Method table */
static PyMethodDef exampleMethods[] = {
    {
        "fib",
        example_fib,
        METH_VARARGS,
        "A method to get the nth value of the Fibonacci sequence"
    },
    {
        "getQuinnStr",                      /* Method name */
        (PyCFunction)example_getQuinnStr,                /* Method function (defined above) */
        METH_NOARGS,                        /* Method arg type */
        "A method to get \"Quinn\""         /* Method docs */
    },
    {NULL, NULL, 0, NULL}                   /* Required null-terminating method */
};


/* Module definition with methods */
static struct PyModuleDef exampleModule = {
    PyModuleDef_HEAD_INIT,                  /* Necessary start of module */
    "example",                              /* Module name */
    "An example module",                    /* Doc string */
    -1,                                     /* size of  per-interpreter state of module, 
                                               or -1 if the module keeps state in global variables. */
    exampleMethods                          /* Module method table */
};


PyMODINIT_FUNC PyInit_example() {
    return PyModule_Create(&exampleModule);
}
