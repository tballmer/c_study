#include "defines.h"
#include "code.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// Constructor for Code.
Code code_init(void) {
    Code c;
    c.top = 0;
    for (uint32_t i = 0; i < MAX_CODE_SIZE; i++) {
        c.bits[i] = 0;
    }
    return c;
}

// Returns the size of Code.
uint32_t code_size(Code *c) {
    return c->top;
}

// Returns whether Code is empty.
bool code_empty(Code *c) {
    return c->top == 0;
}

// Returns whether Code is full;
bool code_full(Code *c) {
    return c->top == ALPHABET;
}

// Sets the bit at given index to 1.
bool code_set_bit(Code *c, uint32_t i) {
    if (i >= ALPHABET) {
        return false;
    }
    c->bits[i / 8] |= (1 << (i % 8));
    return true;
}

// Sets the bit at given index to 0.
bool code_clr_bit(Code *c, uint32_t i) {
    if (i >= ALPHABET) {
        return false;
    }
    c->bits[i / 8] &= ~(1 << (i % 8));
    return true;
}

// Returns the value of the bit at given index.
bool code_get_bit(Code *c, uint32_t i) {
    if (i >= ALPHABET) {
        return false;
    }
    uint8_t t_bit = c->bits[i / 8];
    return t_bit & (1 << (i % 8));
}

// Pushes a bit of given value onto Code.
bool code_push_bit(Code *c, uint8_t bit) {
    if (code_full(c)) {
        return false;
    }
    if (bit) {
        code_set_bit(c, c->top);
    }
    c->top += 1;
    return true;
}

// Pops a bit from Code.
bool code_pop_bit(Code *c, uint8_t *bit) {
    if (code_empty(c)) {
        return false;
    }
    *bit = code_get_bit(c, --c->top);
    code_clr_bit(c, c->top);
    return true;
}

// Prints the entire Code. Useful for debugging.
void code_print(Code *c) {
    for (int i = 0; i < MAX_CODE_SIZE; i++) {
        printf("%" PRIu32 " ", c->bits[i]);
    }
    printf("\n");
}
