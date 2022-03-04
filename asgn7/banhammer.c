#include "bv.h"
#include "bf.h"
#include "node.h"
#include "bst.h"
#include "ht.h"
#include "speck.h"
#include "salts.h"
#include "parser.h"
#include "pq.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>

#define OPTIONS "ht:f:s"
#define WORD    "([a-zA-Z0-9'_]+-)*[a-zA-Z0-9'_]+"

void print_synopsis(void);
void print_tc_c(void);
void print_tc(void);
void print_c(void);

int main(int argc, char **argv) {
    int opt = 0;
    uint32_t table_size = 65536; //2^16
    uint32_t filter_size = 1048576; //2^20
    bool stats = false;
    bool help = false;

    // Reads in CLI options.
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help = true; break;
        case 't': table_size = (uint32_t) atol(optarg); break;
        case 'f': filter_size = (uint32_t) atol(optarg); break;
        case 's': stats = true; break;
        }
    }

    // Error handling
    if (optopt || help) {
        print_synopsis();
        exit(0);
    }

    // Initialize bloom filter and hash table
    BloomFilter *bf = bf_create(filter_size);
    HashTable *ht = ht_create(table_size);
    char oldspeak[256];
    char newspeak[256];

    // Open input files
    FILE *badspeak_input;
    FILE *newspeak_input;
    badspeak_input = fopen("badspeak.txt", "r+");
    newspeak_input = fopen("newspeak.txt", "r+");
    uint32_t bad_length = 0;
    uint32_t old_length = 0;

    // Load badspeak into bloom filter and hash table.
    while (fscanf(badspeak_input, "%s\n", oldspeak) != EOF) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, NULL);
        bad_length++;
    }

    // Load oldspeak and newspeak into bloom filter and hash table.
    while (fscanf(newspeak_input, "%s %s\n", oldspeak, newspeak) != EOF) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, newspeak);
        old_length++;
    }

    // Credit: Assignment Document
    // Compiles regex
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        fclose(badspeak_input);
        fclose(newspeak_input);
        ht_delete(&ht);
        bf_delete(&bf);
        exit(1);
    }

    // Initializes priority queues for storage of bad words.
    PriorityQueue *bad_list = pq_create(bad_length);
    PriorityQueue *old_list = pq_create(old_length);
    char *word = NULL;

    // Utilizes regex to enqueue matching words into proper queue.
    while ((word = next_word(stdin, &re)) != NULL) {
        for (size_t i = 0; i < strlen(word); i++) {
            word[i] = tolower(word[i]);
        }
        if (bf_probe(bf, word)) {
            Node *p;
            p = ht_lookup(ht, word);
            if (p && !p->newspeak) {
                enqueue(bad_list, p);
            } else if (p && p->newspeak) {
                enqueue(old_list, p);
            }
        }
    }

    // Sorts queues
    heap_sort(bad_list, pq_size(bad_list));
    heap_sort(old_list, pq_size(old_list));

    if (stats) {
        printf("Average BST size: %.6f\n", ht_avg_bst_size(ht));
        printf("Average BST height: %.6f\n", ht_avg_bst_height(ht));
        printf("Average branches traversed: %.6f\n", (double) branches / (double) lookups);
        printf("Hash table load: %.6f%%\n", 100.00 * (double) ht_count(ht) / (double) ht_size(ht));
        printf(
            "Bloom filter load: %.6f%%\n", 100.00 * (double) bf_count(bf) / (double) bf_size(bf));
    } else if (!pq_empty(bad_list) && !pq_empty(old_list)) {
        print_tc_c();
        pq_print(bad_list);
        pq_print(old_list);
    } else if (!pq_empty(bad_list) && pq_empty(old_list)) {
        print_tc();
        pq_print(bad_list);
    } else if (pq_empty(bad_list) && !pq_empty(old_list)) {
        print_c();
        pq_print(old_list);
    }

    clear_words();
    regfree(&re);
    fclose(badspeak_input);
    fclose(newspeak_input);
    pq_delete(&bad_list);
    pq_delete(&old_list);
    ht_delete(&ht);
    bf_delete(&bf);
    return 0;
}

// Credit: Reference Program
void print_synopsis(void) {
    printf("SYNOPSIS\n");
    printf("   A word filtering program for the GPRSC.\n");
    printf("   Filters out and reports bad words parsed from stdin.\n");
    printf("\n");
    printf("USAGE\n");
    printf("   ./banhammer [-hs] [-t size] [-f size] \n");
    printf("\n");
    printf("OPTIONS\n");
    printf("   -h              Display program help and usage.\n");
    printf("   -s              Display program statistics.\n");
    printf("   -t size         Specify hash table size (default: 2^16).\n");
    printf("   -f size         Specify bloom filter size (default: 2^20).\n");
}

void print_tc_c(void) {
    printf("Dear beloved citizen of the GPRSC,\n");
    printf("\n");
    printf("We have some good news, and we have some bad news.\n");
    printf("The good news is that there is bad news. The bad news is that you will\n");
    printf("be sent to joycamp and subjected to a week-long destitute existence.\n");
    printf("This is the penalty for using degenerate words, as well as using\n");
    printf("oldspeak in place of newspeak. We hope you can correct your behavior.\n");
    printf("\n");
    printf("Your transgressions, followed by the words you must think on:\n");
    printf("\n");
}

void print_tc(void) {
    printf("Dear beloved citizen of the GPRSC,\n");
    printf("\n");
    printf("You have been caught using degenerate words that may cause\n");
    printf("distress among the moral and upstanding citizens of the GPSRC.\n");
    printf("As such, you will be sent to joycamp. It is there where you will\n");
    printf("sit and reflect on the consequences of your choice in language.\n");
    printf("\n");
    printf("Your transgressions:\n");
    printf("\n");
}

void print_c(void) {
    printf("Dear beloved citizen of the GPRSC,\n");
    printf("\n");
    printf("We recognize your efforts in conforming to the language standards\n");
    printf("of the GPSRC. Alas, you have been caught uttering questionable words\n");
    printf("and thinking unpleasant thoughts. You must correct your wrongspeak\n");
    printf("and badthink at once. Failure to do so will result in your deliverance\n");
    printf("to joycamp.\n");
    printf("\n");
    printf("Words that you must think on:\n");
    printf("\n");
}
