#include "stack.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Stack structure is taken from the assignment document.
struct Stack {
    uint32_t top; // Index of the next empty slot.
    uint32_t capacity; // Number of items that can be pushed.
    uint32_t *items; // Array of items, each with uint32_t.
};

// stack_create() is taken from the assignement document.
// stack_create() is the constructor for the stack structure.
Stack *stack_create(uint32_t capacity) {
    // Dynamically allocates memory for the stack.
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        // Dyamically allocates memory for the items array.
        s->items = (uint32_t *) calloc(capacity, sizeof(uint32_t));
        if (!s->items) { // Error handling.
            free(s);
            s = NULL;
        }
    }
    return s;
}

// stack_delete() is taken from the assignment document.
// stack_delete() is the destructor for stack.
void stack_delete(Stack **s) {
    if (*s && (*s)->items) { // Error handling.
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}

// Returns the size of the stack.
uint32_t stack_size(Stack *s) {
    return s->top;
}

// Checks if the stack is empty.
bool stack_empty(Stack *s) {
    if (s->top > 0) {
        return false;
    }
    return true;
}

// Checks if the stack is full.
bool stack_full(Stack *s) {
    if (s->top == s->capacity) {
        return true;
    }
    return false;
}

// Pushes a vertex onto the check.
bool stack_push(Stack *s, uint32_t x) {
    if (stack_full(s)) { // Makes sure the stack isn't full.
        return false;
    }
    s->items[s->top++] = x;
    return true;
}

// Pops a vertex off the stack and assigns the value to x.
// Passes a pointer to x which fakes pass-by-reference.
bool stack_pop(Stack *s, uint32_t *x) {
    if (stack_empty(s)) { // Makes sure stack isn't empty.
        return false;
    }
    // Credit Christian and assignment document.
    *x = s->items[--s->top];
    return true;
}

// Acts like stack_pop but does not change the stack.
bool stack_peek(Stack *s, uint32_t *x) {
    if (stack_empty(s)) {
        return false;
    }
    *x = s->items[s->top - 1];
    return true;
}

// Copies the src stack into the dst stack,
// but assumes that dst is properly initialized.
void stack_copy(Stack *dst, Stack *src) {
    for (uint32_t i = 0; i < src->top; i++) {
        dst->items[i] = src->items[i];
    }
    dst->top = src->top;
}

// stack_print() is taken from the the assignment document.
// Prints out the stack and arrows in between each desitination.
void stack_print(Stack *s, FILE *outfile, char *cities[]) {
    for (uint32_t i = 0; i < s->top; i++) {
        fprintf(outfile, "%s", cities[s->items[i]]);
        if (i + 1 != s->top) {
            fprintf(outfile, " -> ");
        }
    }
    fprintf(outfile, "\n");
}
