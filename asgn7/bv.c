#include "bv.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

// Structure definition includes length and the bit vector value itself.
struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// Constructs the BitVector ADT.
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv) {
        bv->length = length;
        bv->vector = (uint8_t *) calloc((length / 8) + (length % 8), sizeof(uint8_t));
        if (!bv->vector) {
            free(bv);
            bv = NULL;
        }
    }
    return bv;
}

// Destructor for the BitVector ADT.
void bv_delete(BitVector **bv) {
    if (*bv && (*bv)->vector) {
        free((*bv)->vector);
        free(*bv);
        *bv = NULL;
    }
}

// Returns the length of the BitVector.
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

// Sets a bit at index i.
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i >= bv_length(bv)) {
        return false;
    }
    bv->vector[i / 8] |= (1 << (i % 8));
    return true;
}

// Clears a bit at index i.
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i >= bv_length(bv)) {
        return false;
    }
    bv->vector[i / 8] &= ~(1 << (i % 8));
    return true;
}

// Returns a bit at index i in boolean form.
bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (i >= bv_length(bv)) {
        return false;
    }
    uint8_t bit = bv->vector[i / 8];
    return bit & (1 << (i % 8));
}

// Debugging value useful to see the bits in the BitVector.
void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv_length(bv); i++) {
        printf("%d", bv_get_bit(bv, i));
        if ((i % 8) == 7) {
            printf("\n");
        }
    }
}
