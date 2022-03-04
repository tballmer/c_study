#include "huffman.h"
#include "node.h"
#include "pq.h"
#include "code.h"
#include "stack.h"
#include "defines.h"
#include "header.h"

#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

// Credit: Assignment Document
// Constructs a Huffman tree from given histogram.
Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *pq = pq_create(ALPHABET);
    Node *left;
    Node *right;
    Node *parent;
    Node *root;

    // Searches through histogram for symbols to add.
    // Creates and enqueues Node when found.
    for (int i = 0; i < ALPHABET; i++) {
        if (hist[i] > 0) {
            Node *temp = node_create(i, hist[i]);
            enqueue(pq, temp);
        }
    }

    // Dequeues and joins child nodes until the only the root
    // node is left.
    while (pq_size(pq) > 1) {
        dequeue(pq, &left);
        dequeue(pq, &right);
        parent = node_join(left, right);
        enqueue(pq, parent);
    }

    dequeue(pq, &root);
    pq_delete(&pq);
    return root;
}

// Credit: Assignment Document
// Helper function for build_codes.
void code_helper(Node *root, Code table[static ALPHABET], Code *c) {
    uint8_t bit = 0;
    if (root != NULL) {
        if (root->left == NULL && root->right == NULL) {
            table[root->symbol] = *c;
        } else {
            code_push_bit(c, 0);
            code_helper(root->left, table, c);
            code_pop_bit(c, &bit);

            code_push_bit(c, 1);
            code_helper(root->right, table, c);
            code_pop_bit(c, &bit);
        }
    }
}

// Credit: Assignment Document
// Populates the given code table by traversing through the Huffman Tree
// accessed by using the given root node.
void build_codes(Node *root, Code table[static ALPHABET]) {
    Code c = code_init();
    code_helper(root, table, &c);
}

// Credit: Assignment Document
// Writes the contents of the Huffman tree to given outfile
// by conducting a post-order traversal of the tree defined by root.
void dump_tree(int outfile, Node *root) {
    if (root) {
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);

        if (root->left == NULL && root->right == NULL) {
            write(outfile, "L", 1);
            uint8_t s_buf[1] = { root->symbol };
            write(outfile, s_buf, 1);
        } else {
            write(outfile, "I", 1);
        }
    }
}

// Credit: Assignment Document
// Reconstructs the Huffman tree from given tree_dump through
// a post-order traversal.
Node *rebuild_tree(uint16_t nbytes, uint8_t tree_dump[static nbytes]) {
    Stack *s = stack_create(nbytes);
    Node *root;
    for (int i = 0; i < nbytes; i++) {
        if (tree_dump[i] == 'I') {
            Node *right;
            Node *left;
            Node *parent;
            stack_pop(s, &right);
            stack_pop(s, &left);
            parent = node_join(left, right);
            stack_push(s, parent);
        }
        if (tree_dump[i] == 'L') {
            i += 1;
            Node *n = node_create(tree_dump[i], 0);
            stack_push(s, n);
        }
    }
    stack_pop(s, &root);
    return root;
}
