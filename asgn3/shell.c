#include "shell.h"

#include "stats.h"

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

// shell_sort() is based on Professor Long's Python pseudocode
// found in the assignment 3 pdf.
void shell_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t range = log(2 * n + 3) / log(3);
    uint32_t gap;
    uint32_t j;
    uint32_t temp;

    do {
        gap = (pow(3, range) - 1) / 2;
        for (uint32_t i = gap; i < n; i++) {
            j = i;
            temp = move(stats, A[i]);
            while (j >= gap && cmp(stats, A[j - gap], temp) == 1) {
                A[j] = move(stats, A[j - gap]);
                j -= gap;
            }
            A[j] = move(stats, temp);
        }
    } while (--range > 0);
}
