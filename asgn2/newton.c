#include "mathlib.h"

#include <stdio.h>
#include <stdlib.h>

static int num_of_iters = 0;

// This program is based on Professor Long's sqrt pseudocode
// found in the assignment 2 pdf. I simply converted his python
// code into C and kept track of the iterations.

double sqrt_newton(double x) {
    num_of_iters = 0;
    double z = 0.0;
    double y = 1.0;
    while (EPSILON < absolute(y - z)) {
        z = y;
        y = 0.5 * (z + x / z);
        num_of_iters++;
    }
    return y;
}

// sqrt_newton_iters() is a utlity function which returns the
// number of iterations is took for sqrt_newton to adequately
// approximate the square root.

int sqrt_newton_iters(void) {
    return num_of_iters;
}
