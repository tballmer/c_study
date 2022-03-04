#include "insert.h"

#include "stats.h"

#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

// insertion_sort() is based on Professor Long's Python pseudocode
// found in the assignment 3 pdf.
void insertion_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t j;
    uint32_t temp;
    for (uint32_t i = 1; i < n; i++) {
        j = i;
        temp = move(stats, A[i]);
        while (j > 0 && cmp(stats, A[j - 1], temp) == 1) {
            A[j] = move(stats, A[j - 1]);
            j--;
        }
        A[j] = move(stats, temp);
    }
}
