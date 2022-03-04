#include "mathlib.h"

#include <stdio.h>
#include <stdlib.h>

static int num_of_terms = 0;

// e's approximation may be one of the more simple approximations in this assignment,
// but the program still needs to watch out for the first term (0! = 1). Other than
// that, the calculation 1 / k! is easily handled with a few variables.

double e(void) {
    double current_value = 1.0, factorial_value = 1.0;
    double total = 0.0;

    for (double i = 0; EPSILON < current_value; i++) {
        if (i == 0) {
            factorial_value = 1.0;
            current_value = 1.0;
        } else {
            factorial_value *= i;
            current_value = 1 / factorial_value;
        }
        num_of_terms++;
        total += current_value;
    }

    return total;
}

// e_terms is a utlity function which returns the number of terms it took
// e() to adequately approximate euler's number.

int e_terms(void) {
    return num_of_terms;
}
