#include "ht.h"
#include "node.h"
#include "bst.h"
#include "salts.h"
#include "speck.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

uint64_t lookups = 0;

// The HashTable struct defines a salt, size, and Node array.
struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

// Constructor for the HashTable ADT.
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
        ht->size = size;
        ht->trees = (Node **) calloc(size, sizeof(Node *));
        if (!ht->trees) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

// Returns the size of the hash table.
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

// Debugging function useful for viewing the values in the hash table.
void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht_size(ht); i++) {
        if (ht->trees[i]) {
            printf("Tree %" PRIu32 "\n", i);
            bst_print(ht->trees[i]);
        }
    }
}

// Destructor for the HashTable.
void ht_delete(HashTable **ht) {
    for (uint32_t i = 0; i < ht_size(*ht); i++) {
        bst_delete(&(*ht)->trees[i]);
    }
    free((*ht)->trees);
    free(*ht);
    *ht = NULL;
}

// Inserts a node into the hash table.
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    lookups++;
    uint32_t hash_value = hash(ht->salt, oldspeak) % ht_size(ht);
    if (ht->trees[hash_value] == NULL) {
        Node *root = bst_create();
        ht->trees[hash_value] = bst_insert(root, oldspeak, newspeak);
    } else {
        bst_insert(ht->trees[hash_value], oldspeak, newspeak);
    }
}

// Returns a node in the hash table given an oldspeak input.
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    lookups++;
    uint32_t hash_value = hash(ht->salt, oldspeak) % ht_size(ht);
    return bst_find(ht->trees[hash_value], oldspeak);
}

// Returns the count of the hash table.
uint32_t ht_count(HashTable *ht) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht_size(ht); i++) {
        if (ht->trees[i]) {
            count++;
        }
    }
    return count;
}

// Returns the average bst size of the hash table.
double ht_avg_bst_size(HashTable *ht) {
    double avg_size = 0.0;
    for (uint32_t i = 0; i < ht_size(ht); i++) {
        if (ht->trees[i]) {
            avg_size += (double) bst_size(ht->trees[i]);
        }
    }
    return avg_size / ht_count(ht);
}

// Returns the average best height of the hash table.
double ht_avg_bst_height(HashTable *ht) {
    double avg_height = 0.0;
    for (uint32_t i = 0; i < ht_size(ht); i++) {
        if (ht->trees[i]) {
            avg_height += (double) bst_height(ht->trees[i]);
        }
    }
    return avg_height / ht_count(ht);
}
