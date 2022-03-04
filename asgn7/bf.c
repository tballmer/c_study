#include "bf.h"
#include "salts.h"
#include "bv.h"
#include "speck.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

// The BloomFilter struct defines 3 distinct salts and a bit vector.
struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

// Constructs the BloomFilter ADT.
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        bf->primary[0] = SALT_PRIMARY_LO;
        bf->primary[1] = SALT_PRIMARY_HI;
        bf->secondary[0] = SALT_SECONDARY_LO;
        bf->secondary[1] = SALT_SECONDARY_HI;
        bf->tertiary[0] = SALT_TERTIARY_LO;
        bf->tertiary[1] = SALT_TERTIARY_HI;
        bf->filter = bv_create(size);
    }
    return bf;
}

// Destructs the BloomFilter ADT.
void bf_delete(BloomFilter **bf) {
    if (*bf && (*bf)->filter) {
        bv_delete(&(*bf)->filter);
        free(*bf);
        *bf = NULL;
    }
}

// Returns the size of the BloomFilter.
uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

// Inserts a word into the BloomFilter by hash by inserting the words three distinct hashes.
void bf_insert(BloomFilter *bf, char *oldspeak) {
    uint32_t hash_value = hash(bf->primary, oldspeak) % bf_size(bf);
    bv_set_bit(bf->filter, hash_value);

    hash_value = hash(bf->secondary, oldspeak) % bf_size(bf);
    bv_set_bit(bf->filter, hash_value);

    hash_value = hash(bf->tertiary, oldspeak) % bf_size(bf);
    bv_set_bit(bf->filter, hash_value);
}

// Checks to see if a word is propbably badspeak.
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    uint32_t hash_1 = hash(bf->primary, oldspeak) % bf_size(bf);
    uint32_t hash_2 = hash(bf->primary, oldspeak) % bf_size(bf);
    uint32_t hash_3 = hash(bf->primary, oldspeak) % bf_size(bf);
    if (bv_get_bit(bf->filter, hash_1) && bv_get_bit(bf->filter, hash_2)
        && bv_get_bit(bf->filter, hash_3)) {
        return true;
    }
    return false;
}

// Returns the number of set bits in the BloomFilter.
uint32_t bf_count(BloomFilter *bf) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < bf_size(bf); i++) {
        count += (uint32_t) bv_get_bit(bf->filter, i);
    }
    return count;
}

// Debugging function useful to see set bits in the BloomFilter.
void bf_print(BloomFilter *bf) {
    for (uint32_t i = 0; i < bf_size(bf); i++) {
        printf("%d", bv_get_bit(bf->filter, i));
        if ((i % 8) == 7) {
            printf("\n");
        }
    }
}
