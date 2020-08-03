#include "../include/random.h"

static double next_uniform() {
    return random() / (double) RAND_MAX;
}

static double next_gauss() {
    /*
        Source:
        https://github.com/python/cpython/blob/master/Lib/random.py
        https://linux.die.net/man/3/random
        cos(2*pi*x)*sqrt(-2*log(1-y))
        sin(2*pi*x)*sqrt(-2*log(1-y))
    */
    if (gauss_has_next) {
        gauss_has_next = false;
        return gauss_next;
    } else {
        double x2pi = next_uniform() * TWO_PI;
        double g2rad = sqrt(-2.0 * log(1.0 - next_uniform()));
        gauss_next = sin(x2pi) * g2rad;
        gauss_has_next = true;
        return cos(x2pi) * g2rad;
    }
}

static double uniform(const double lower, const double upper) {
    // No checking for lower <= upper
    return next_uniform() * (upper - lower) + lower;
}

static double gauss(const double mu, const double sigma) {
    return next_gauss() * sigma + mu;
}