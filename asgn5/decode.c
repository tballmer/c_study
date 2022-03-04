#include "node.h"
#include "pq.h"
#include "code.h"
#include "defines.h"
#include "io.h"
#include "stack.h"
#include "huffman.h"
#include "header.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

#define OPTIONS "hi:o:v"

void print_help(void);
void traverse_tree(int infile, Node *root, int outfile, uint64_t *symbols_written);

int main(int argc, char **argv) {
    int infile = 0;
    int outfile = 1;
    int opt = 0;
    bool help = false;
    bool stats = false;
    uint8_t buf[BLOCK];

    // Cycles through CLI options
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': infile = open(optarg, O_RDONLY); break;
        case 'o': outfile = open(optarg, O_RDWR | O_CREAT, 0666); break;
        case 'h': help = true; break;
        case 'v': stats = true; break;
        }
    }

    // Prints help if CLI options are incorrect or if user prompted help.
    if (optopt || help) {
        print_help();
        close(infile);
        close(outfile);
        exit(0);
    }

    struct stat s;
    fstat(infile, &s);

    // If file is empty exit the program.
    if (s.st_size == 0) {
        close(infile);
        close(outfile);
        exit(0);
    }

    // Reads header
    read_bytes(infile, buf, 16);

    // If magic number does not match print an error and exit.
    uint32_t infile_magic = (buf[0]) | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
    if (infile_magic != MAGIC) {
        // Credit: Reference Program
        printf("Error: unable to read header.\n");
        exit(1);
    }

    fchmod(outfile, s.st_mode);

    uint16_t tree_size = (buf[6]) | (buf[7] << 8);
    uint64_t file_size = (buf[8]) | (buf[9] << 8) | (buf[10] << 16) | (buf[11] << 24)
                         | ((uint64_t) buf[12] << 32) | ((uint64_t) buf[13] << 40)
                         | ((uint64_t) buf[14] << 48) | ((uint64_t) buf[15] << 56);
    uint8_t tree_dump[tree_size];

    // Reads through the Huffman Tree.
    read_bytes(infile, tree_dump, tree_size);

    // Recontructs the Huffman Tree.
    Node *root = rebuild_tree(tree_size, tree_dump);

    // Prints the symbols by traversing through the Huffman Tree
    // given the codes.
    uint64_t symbols_written = 0;
    while (symbols_written < file_size) {
        traverse_tree(infile, root, outfile, &symbols_written);
    }

    // Credit: Reference Program.
    // Prints the stats if user indicated.
    if (stats) {
        printf("Compressed file size: %ld bytes\n", s.st_size);
        printf("Decompressed file size: %ld bytes\n", file_size);
        printf(
            "Space saving: %.2f%% \n", 100.00 * (1.00 - ((float) s.st_size / (float) file_size)));
    }

    close(infile);
    close(outfile);

    return 0;
}

// Traverses through the Huffman tree by the codes directions.
// When a leaf is found its symbol is printed.
void traverse_tree(int infile, Node *root, int outfile, uint64_t *symbols_written) {
    uint8_t bit = 0;
    read_bit(infile, &bit);
    if (bit == 0) {
        if (root->left) {
            if (root->left->left == NULL && root->left->right == NULL) {
                write(outfile, &(root->left->symbol), 1);
                (*symbols_written)++;
                return;
            } else {
                traverse_tree(infile, root->left, outfile, symbols_written);
            }
        }
    } else if (bit == 1) {
        if (root->right) {
            if (root->right->left == NULL && root->right->right == NULL) {
                write(outfile, &(root->right->symbol), 1);
                (*symbols_written)++;
                return;
            } else {
                traverse_tree(infile, root->right, outfile, symbols_written);
            }
        }
    }
}

// Prints the help manual.
void print_help(void) {
    printf("SYNOPSIS\n");
    printf("  Decompresses a file using the Huffman coding algorithm.\n");
    printf("\n");
    printf("USAGE\n");
    printf("  ./decode [-h] [-i infile] [-o outfile]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("  -h           Program usage and help.\n");
    printf("  -v           Prints compression statistics.\n");
    printf("  -i infile    Input file to decompress.\n");
    printf("  -o outfile   Output of compressed data.\n");
}
