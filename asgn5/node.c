#include "node.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

// Constructor for Node
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->symbol = symbol;
    n->frequency = frequency;
    return n;
}

// Destructor for node
void node_delete(Node **n) {
    free(*n);
    *n = NULL;
}

// Joins a left child and right child together to create a new parent node.
Node *node_join(Node *left, Node *right) {
    Node *pn = node_create('$', left->frequency + right->frequency);
    pn->left = left;
    pn->right = right;
    return pn;
}

// Prints the node. Useful for debugging.
void node_print(Node *n) {
    printf("Frequency: %" PRIu64 "\n", n->frequency);
    printf("Symbol: %" PRIu8 "\n", n->symbol);
    if (n->left) {
        node_print(n->left);
    }
    if (n->right) {
        node_print(n->right);
    }
}
