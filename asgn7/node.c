#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constructor for the Node ADT.
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) calloc(1, sizeof(Node));
    if (n) {
        if (oldspeak) {
            n->oldspeak = strdup(oldspeak);
        } else {
            n->oldspeak = NULL;
        }

        if (newspeak) {
            n->newspeak = strdup(newspeak);
        } else {
            n->newspeak = NULL;
        }
    }
    return n;
}

// Destructor for the Node ADT.
void node_delete(Node **n) {
    if ((*n)->oldspeak) {
        free((*n)->oldspeak);
    }
    if ((*n)->newspeak) {
        free((*n)->newspeak);
    }
    free(*n);
    *n = NULL;
}

// Debugging function used to print out a node.
void node_print(Node *n) {
    if (n->oldspeak && n->newspeak) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    } else if (n->newspeak == NULL) {
        printf("%s\n", n->oldspeak);
    }
}
