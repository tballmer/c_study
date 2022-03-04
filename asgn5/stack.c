#include "stack.h"
#include "node.h"

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Credit: Assignment Document
// Structure for Stack.
struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

// Constructor for Stack.
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->capacity = capacity;
        s->top = 0;
        s->items = (Node **) malloc(capacity * sizeof(Node *));
    }
    return s;
}

// Destructor for Stack.
void stack_delete(Stack **s) {
    free((*s)->items);
    free(*s);
    *s = NULL;
}

// Returns whether Stack is empty.
bool stack_empty(Stack *s) {
    return s->top == 0;
}

// Returns whether Stack is full.
bool stack_full(Stack *s) {
    return s->top == s->capacity;
}

// Returns the number of items in Stack.
uint32_t stack_size(Stack *s) {
    return s->top;
}

// Pushes a given Node onto Stack.
bool stack_push(Stack *s, Node *n) {
    if (stack_full(s)) {
        return false;
    }
    s->items[s->top++] = n;
    return true;
}

// Pops a Node from Stack.
bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s)) {
        return false;
    }
    *n = s->items[--(s->top)];
    return true;
}

// Prints the contents of Stack. Useful for debugging.
void stack_print(Stack *s) {
    for (uint32_t i = 0; i < s->top; i++) {
        printf("Index: %" PRIu32 "\n", i);
        printf("Symbol: %" PRIu32 "\n", s->items[i]->symbol);
        printf("Frequency: %" PRIu64 "\n", s->items[i]->frequency);
    }
}
