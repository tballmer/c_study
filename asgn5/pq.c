#include "pq.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure for PriorityQueue.
struct PriorityQueue {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

// Constructor for PriorityQueue.
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (q) {
        q->top = 0;
        q->capacity = capacity;
        // Credit for items initialization: Christian
        q->items = (Node **) malloc(capacity * sizeof(Node *));
    }
    return q;
}

// Destructor for PriorityQueue.
void pq_delete(PriorityQueue **q) {
    free((*q)->items);
    free(*q);
    *q = NULL;
}

// Returns whether PriorityQueue is empty.
bool pq_empty(PriorityQueue *q) {
    if (q->top) {
        return false;
    }
    return true;
}

// Returns whether PriorityQueue is full.
bool pq_full(PriorityQueue *q) {
    if (q->top == q->capacity) {
        return true;
    }
    return false;
}

// Returns the size of PriorityQueue.
uint32_t pq_size(PriorityQueue *q) {
    return q->top;
}

// Credit: Christian
// Helper function to get a PriorityQueue element.
static Node *get_pq_element(PriorityQueue *q, uint32_t index) {
    return q->items[index];
}

// Credit: Christian
// Helper function to set a PriorityQueue element to a node.
static void set_pq_element(PriorityQueue *q, uint32_t index, Node *n) {
    q->items[index] = n;
}

// Returns the child of a parent with the least value.
uint32_t min_child(PriorityQueue *q, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if (right <= last && q->items[right - 1]->frequency < q->items[left - 1]->frequency) {
        return right;
    }
    return left;
}

// Fixes the min heap.
void fix_heap(PriorityQueue *q, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t mother = first;
    uint32_t great = min_child(q, mother, last);
    while (mother <= last / 2 && !found) {
        if (q->items[great - 1]->frequency < q->items[mother - 1]->frequency) {
            Node *t = get_pq_element(q, mother - 1);
            set_pq_element(q, mother - 1, get_pq_element(q, great - 1));
            set_pq_element(q, great - 1, t);
            mother = great;
            great = min_child(q, mother, last);
        } else {
            found = true;
        }
    }
}

// Enqueues a node into the PriorityQueue
bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q)) {
        return false;
    }
    q->items[q->top++] = n;
    for (uint32_t father = q->top / 2; father > 0; father--) {
        fix_heap(q, father, q->top);
    }
    return true;
}

// Dequeues a node with the greates priority from the PriorityQueue.
bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q)) {
        return false;
    }
    *n = get_pq_element(q, 0);
    q->items[0] = q->items[--(q->top)];
    for (uint32_t father = q->top / 2; father > 0; father--) {
        fix_heap(q, father, q->top);
    }
    return true;
}

// Prints the entire PriorityQueue. Useful for debugging.
void pq_print(PriorityQueue *q) {
    for (uint32_t i = 0; i < q->top; i++) {
        printf("Index: %" PRIu32 "\n", i);
        printf("Symbol: %" PRIu8 "\n", q->items[i]->symbol);
        printf("Frequency: %" PRIu64 "\n", q->items[i]->frequency);
    }
}
