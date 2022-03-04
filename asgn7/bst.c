#include "bst.h"

#include <string.h>

uint64_t branches = 0;

// Constructor for the binary search tree.
Node *bst_create(void) {
    return NULL;
}

// Credit: Professor Long's lecture 18 slides.
// Destructor for the binary seach tree.
void bst_delete(Node **root) {
    if (*root) {
        bst_delete(&(*root)->left);
        bst_delete(&(*root)->right);
        node_delete(root);
    }
}

// Credit: Professor Long's lecture 18 slides.
// Utlility function to find the maxium value of two integers.
static int max(uint32_t x, uint32_t y) {
    return x > y ? x : y;
}

// Credit: Professor Long's lecture 18 slides.
// Returns the height of the binary search tree.
uint32_t bst_height(Node *root) {
    if (root) {
        return 1 + max(bst_height(root->left), bst_height(root->right));
    }
    return 0;
}

// Returns the size of the binary search tree.
uint32_t bst_size(Node *root) {
    if (root) {
        return 1 + bst_size(root->left) + bst_size(root->right);
    }
    return 0;
}

// Credit: Professor Long's lecture 18 slides.
// Returns a node in the bst of a given oldspeak word. If none exists, return NULL.
Node *bst_find(Node *root, char *oldspeak) {
    if (root) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            return bst_find(root->left, oldspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            return bst_find(root->right, oldspeak);
        }
        branches++;
    }
    return root;
}

// Credit: Professor Long's lecture 18 slides.
// Inserts a node into the bst.
Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    if (root) {
        if (strcmp(root->oldspeak, oldspeak) == 0) {
            return root;
        }
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            root->left = bst_insert(root->left, oldspeak, newspeak);
        } else {
            root->right = bst_insert(root->right, oldspeak, newspeak);
        }
        branches++;
        return root;
    }
    return node_create(oldspeak, newspeak);
}

// Credit: Professor Long's lecture 18 slides
// Debugging function used to see values in the bst.
void bst_print(Node *root) {
    if (root) {
        bst_print(root->left);
        node_print(root);
        bst_print(root->right);
    }
}
