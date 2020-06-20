
#include "quantum_types.h"

typedef struct {
    T *start;
    T *stop;
    T *ptr;
    long step;
} IterState;

/* All operations are unchecked */
static void IterState_init(IterState *is, T* start, long len, long step) {
    is->start = start;
    is->stop = start + len;
    is->ptr = start;
    is->step = step;
}

static T* IterState_next(IterState *is) {
    T* ret = is->ptr;
    is->ptr += is->step;
    return ret;
}

static int IterState_has_next(IterState *is) {
    return is->ptr != is->stop;
}

// You can ride the iterator train INDEFINITELY
static void IterState_reset(IterState *is) {
    is->ptr = is->start;
}

