#include "mathlib.h"

#include <stdio.h>
#include <stdlib.h>

static int num_of_terms = 0;

// The madhava solution proved itself to be way more complicated in calculation
// than other formulas. Since -3 has an exponent -i, each new term flips signs.
// This is why absolute() is used on the current_value function. And this
// solution also had to address the first iteration having (-3)^0.

double pi_madhava(void) {
    double current_value, exponent_value, sqrt, total;
    current_value = 1.0;
    total = 0.0;
    sqrt = sqrt_newton(12.0);

    for (int i = 0; EPSILON < absolute(current_value); i++) {
        if (i == 0) {
            exponent_value = 1.0;
            current_value = 1.0;
        } else {
            exponent_value *= (-3);
            current_value = 1 / (exponent_value * (2 * i + 1));
        }
        num_of_terms++;
        total += current_value;
    }
    total *= sqrt;
    return total;
}

// pi_madhava_terms() is a utlity function which returns the number of terms
// it took pi_madhava() to adequately approximate pi.

int pi_madhava_terms(void) {
    return num_of_terms;
}
