#include "quick.h"

#include "stats.h"

#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

// All functions in quick.c are based on Professor Long's python pseudocode
// found in the assignment 3 pdf.

// partition() compares and swaps two values when needed. It returns
// the new partition to be used.
uint32_t partition(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    uint32_t i = lo - 1;
    for (uint32_t j = lo; j < hi; j++) {
        if (cmp(stats, A[hi - 1], A[j - 1]) == 1) {
            i++;
            swap(stats, &A[i - 1], &A[j - 1]);
        }
    }
    swap(stats, &A[i], &A[hi - 1]);
    return i + 1;
}

// quick_sorter() implements the recursion functionality of quick_sort().
void quick_sorter(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    if (lo < hi) {
        uint32_t p = partition(stats, A, lo, hi);
        quick_sorter(stats, A, lo, p - 1);
        quick_sorter(stats, A, p + 1, hi);
    }
}

// quick_sort() itself is used to connect the helper functions.
void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n);
}
