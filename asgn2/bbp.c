#include "mathlib.h"

#include <stdlib.h>

static int num_of_terms = 0;

// I chose to implement the Bailey-Borwein-Plouffe formula in Horner normal form,
// which is why there appears to be many seemingly random numbers.

double pi_bbp(void) {
    double current_value = 1.0;
    double exponent_value;
    double total = 0.0;
    double fraction_value;

    for (double i = 0.0; EPSILON < current_value; i++) {
        fraction_value = ((i * (120.0 * i + 151.0) + 47.0)
                          / (i * (i * (i * (512.0 * i + 1024.0) + 712.0) + 194.0) + 15.0));
        if (i == 0) {
            exponent_value = 1.0;
        } else {
            exponent_value *= (1.0 / 16.0);
        }
        current_value = exponent_value * fraction_value;
        num_of_terms++;
        total += current_value;
    }
    return total;
}

// pi_bbp_terms is a utility function which retreives the number of terms
// it took for pi_bbp to adequetly approximate pi.

int pi_bbp_terms(void) {
    return num_of_terms;
}
