
#include <stdlib.h>
#include <math.h>

/* 2 * pi */
static double TWOPI = M_PI * 2;

typedef struct RandomGenerator {
    int gauss_has_next;
    double gauss_next;
} RandomGenerator;

static void RandomGenerator_init(RandomGenerator *rg) {
    srandom(time(NULL)); // seed is time based
    rg->gauss_has_next = 0;
    rg->gauss_next = 0;
}

static double uniform() {
    return random() / RAND_MAX;
}

static double gauss(RandomGenerator *rg, double mu, double sigma) {
    /*
        Source:
        https://github.com/python/cpython/blob/master/Lib/random.py
        https://linux.die.net/man/3/random
        cos(2*pi*x)*sqrt(-2*log(1-y))
        sin(2*pi*x)*sqrt(-2*log(1-y))
    */
    if (rg->gauss_has_next) {
        rg->gauss_has_next = 0;
        return rg->gauss_next * sigma +  mu;
    } else {
        rg->gauss_has_next = 1;
        double x2pi = uniform() * TWOPI;
        double g2rad = sqrt(-2.0 * log(1.0 - uniform()));
        rg->gauss_next = sin(x2pi) * g2rad;
        return (cos(x2pi) * g2rad) * sigma + mu;
    }
} 