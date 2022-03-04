#include "mathlib.h"

#include <stdio.h>
#include <stdlib.h>

static int num_of_terms = 0;

// In the Euler approximation of pi, the series summation itself is not the
// approximation of pi. The summation needs to be multiplied and square rooted.
// The square root function used is sqrt_newton(), which is defined by myself
// in the newton.c file.

double pi_euler(void) {
    double current_value = 1.0;
    double total = 0.0;

    // I made the iterator i a double because I was having problems with C
    // converting the calculation into an integer.
    for (double i = 1.0; EPSILON < current_value; i++) {
        current_value = 1.0 / (i * i);
        current_value *= 6.0;
        total += current_value;
        num_of_terms++;
    }
    total = sqrt_newton(total);
    return total;
}

// pi_euler_terms is a utility function which returns the number of terms
// it took pi_euler() to adequetly approximate pi.

int pi_euler_terms(void) {
    return num_of_terms;
}
