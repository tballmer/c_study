#include "mathlib.h"

#include <stdlib.h>

static int num_of_factors = 0;

// It made the most sense for me to use recursion to calculate the
// nested square roots because the recusive solution was so simple.

double rec(double x) {
    if (x == 1.0) {
        return sqrt_newton(2.0);
    } else {
        return sqrt_newton(2.0 + rec(x - 1.0));
    }
}

// Viete's formula is unique from the others because it used a product
// rather than a summation. Since stopping the calculation when the current
// value < EPSILON does not apply here. I found the difference between
// the previous factor and current factor and then compared to EPSILON.

double pi_viete(void) {
    double current_value = 0.0;
    double previous_value = 1.0;
    double total = 0.0;

    for (double i = 1.0; EPSILON < (previous_value - current_value); i++) {
        if (i == 1.0) {
            current_value = 1.0;
        } else {
            previous_value = current_value;
        }
        current_value *= (rec(i) / 2.0);
        num_of_factors++;
    }
    total = 2.0 / current_value;
    return total;
}

// pi_viete_factors() is a utility function which returns the number of
// factors it took for pi_viete to adequately approximate pi.

int pi_viete_factors(void) {
    return num_of_factors;
}
