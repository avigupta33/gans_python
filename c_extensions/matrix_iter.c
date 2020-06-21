
#include "quantum_types.h"

typedef struct {
    long rows;
    long cols;
    long size;
    T *start;
    T *end;
    T *ptr;
} MatrixIter;

static void MatrixIter_init(MatrixIter *iter, long rows, long cols, T *data) {
    iter->rows = rows;
    iter->cols = cols;
    iter->size = rows * cols;
    iter->start = data;
    iter->end = data + iter->size;
    iter->ptr = data;
}

static void MatrixIter_reset(MatrixIter *iter) {
    iter->ptr = iter->start;
}

static T* MatrixIter_nextInRow(MatrixIter *iter) {
    T *res = iter->ptr;
    ++iter->ptr;
    return res;
}

static T* MatrixIter_nextInCol(MatrixIter *iter) {
    T *res = iter->ptr;
    iter->ptr += iter->cols;
    if (iter->ptr >= iter->end) {
        iter->ptr -= iter->size - 1;
    }
    return res;
}

