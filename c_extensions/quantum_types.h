
#ifndef QUANTUMDEFS_H
#define QUANTUMDEFS_H

/* Matrix element type */
typedef double T;

/* Composition function to merge two arrays */
typedef void(*compfunc)(T*, T*, T*, Py_ssize_t);

#endif /* QUANTUMDEFS_H */