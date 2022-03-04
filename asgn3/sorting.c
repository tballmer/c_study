#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "aeisqr:n:p:h"

// I used Eugene's lab section demo code to implement sorting.c

typedef enum { HEAP, SHELL, INSERTION, QUICK, USAGE } Sorts;
void synopsis(void);
void print_control(Stats *stats, uint32_t *A, char *s, uint32_t size, uint32_t print_elements);

int main(int argc, char **argv) {
    Set s = empty_set();
    int opt = 0;
    uint32_t seed = 13371453;
    uint32_t size = 100;
    uint32_t print_elements = 100;
    const uint32_t array_mask = 0x3FFFFFFF;
    bool valid_command = false;

    // This while loop reads through CLI options.
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            for (Sorts x = HEAP; x < USAGE; x++) {
                s = insert_set(x, s);
            }
            break;
        case 'e': s = insert_set(HEAP, s); break;
        case 'i': s = insert_set(INSERTION, s); break;
        case 's': s = insert_set(SHELL, s); break;
        case 'q': s = insert_set(QUICK, s); break;
        case 'r': seed = (uint32_t) atol(optarg); break;
        case 'n': size = (uint32_t) atol(optarg); break;
        case 'p': print_elements = (uint32_t) atol(optarg); break;
        case 'h': s = insert_set(USAGE, s); break;
        default: s = insert_set(USAGE, s); break;
        }
    }

    // This for loop decides if the synopsis should be printed.
    for (Sorts i = 0; i < USAGE; i++) {
        if (member_set(i, s)) {
            valid_command = true;
        }
    }

    // This control sequence decides what is printed when user input is incorrect
    if (member_set(USAGE, s) || valid_command == false) {
        if (member_set(USAGE, s)) {
            synopsis();
            exit(0);
        } else {
            printf("Select at least one sort to perform.\n");
            synopsis();
            exit(0);
        }
    }

    srandom(seed);
    Stats stats;
    stats.moves = 0;
    stats.compares = 0;

    // Dynamically allocates the number array and bitmasks each value to 30 bits.
    uint32_t *numbers = (uint32_t *) calloc(size, sizeof(uint32_t));
    for (uint32_t i = 0; i < size; i++) {
        numbers[i] = random() & array_mask;
    }

    // If HEAP option is enabled, this program controls heap_sort()
    // execution, printing, and memory allocation.
    if (member_set(HEAP, s)) {
        uint32_t *heap = (uint32_t *) calloc(size, sizeof(uint32_t));
        for (uint32_t x = 0; x < size; x++) {
            heap[x] = numbers[x];
        }

        heap_sort(&stats, heap, size);
        char *sort = "Heap";
        print_control(&stats, heap, sort, size, print_elements);

        free(heap);
        reset(&stats);
    }

    // If shell sort is enabled, this program controls shell_sort()
    // execution, printing, and memory allocation.
    if (member_set(SHELL, s)) {
        uint32_t *shell = (uint32_t *) calloc(size, sizeof(uint32_t));
        for (uint32_t x = 0; x < size; x++) {
            shell[x] = numbers[x];
        }

        shell_sort(&stats, shell, size);
        char *sort = "Shell";
        print_control(&stats, shell, sort, size, print_elements);

        free(shell);
        reset(&stats);
    }

    // If insertion sort is enabled, this program controls insertion_sort()
    // execution, printing, and memory allocation.
    if (member_set(INSERTION, s)) {
        uint32_t *insertion = (uint32_t *) calloc(size, sizeof(uint32_t));
        for (uint32_t x = 0; x < size; x++) {
            insertion[x] = numbers[x];
        }

        insertion_sort(&stats, insertion, size);
        char *sort = "Insertion";
        print_control(&stats, insertion, sort, size, print_elements);

        free(insertion);
        reset(&stats);
    }

    // If quick sort is enabled, this program controls quick_sort()
    // execution, printing, and memory allocation.
    if (member_set(QUICK, s)) {
        uint32_t *quick = (uint32_t *) calloc(size, sizeof(uint32_t));
        for (uint32_t x = 0; x < size; x++) {
            quick[x] = numbers[x];
        }

        quick_sort(&stats, quick, size);
        char *sort = "Quick";
        print_control(&stats, quick, sort, size, print_elements);

        free(quick);
        reset(&stats);
    }

    free(numbers);
    return 0;
}

// synopsis() prints outs the manual from the reference program
void synopsis(void) {
    printf("SYNOPSIS\n");
    printf("\n");
    printf("A collection of comparison-based sorting algorithms.\n");
    printf("\n");
    printf("USAGE\n");
    printf("./sorting [-aeisqhr:n:p:] [-n length] [-p elements] [-r seed]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("-h              display program help and usage.\n");
    printf("-a              enable all sorts.\n");
    printf("-e              enable Heap Sort.\n");
    printf("-i              enable Insertion Sort.\n");
    printf("-s              enable Shell Sort.\n");
    printf("-q              enable Quick Sort.\n");
    printf("-n length       specify number of array elements (default: 100).\n");
    printf("-p elements     specify number of elements to print (default: 100).\n");
    printf("-r seed         specify random seed (default: 13371453).\n");
}

// print_control() decides how output is printed based on user input.
void print_control(Stats *stats, uint32_t *A, char *s, uint32_t size, uint32_t print_elements) {
    printf("%s Sort, ", s);
    printf("%" PRIu32 " elements, ", size);
    printf("%" PRIu64 " moves, ", stats->moves);
    printf("%" PRIu64 " compares\n", stats->compares);
    if (print_elements > size) {
        print_elements = size;
    }
    int c = 1;
    for (uint32_t i = 0; i < print_elements; i++, c++) {
        printf("%13" PRIu32 "", A[i]);
        if (c == 5 || i + 1 == print_elements) {
            printf("\n");
            c = 0;
        }
    }
}
