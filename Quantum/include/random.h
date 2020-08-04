#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static double TWO_PI = M_PI * 2;

static bool gauss_has_next = false;
static double gauss_next = 0;

static void set_seed(unsigned int seed);

static double next_uniform();
static double next_gauss();

static double uniform(const double lower, const double upper);
static double gauss(const double mu, const double sigma);

#endif  /* RANDOM_H */