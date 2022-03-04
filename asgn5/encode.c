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

int main(int argc, char **argv) {
    // Credit: Eugene and Christian
    int infile = 0;
    int outfile = 1;
    int opt = 0;
    bool help = false;
    bool stats = false;
    uint8_t buf[BLOCK];
    int size = 0;
    uint64_t hist[ALPHABET] = { 0 };
    Code table[ALPHABET] = { 0 };

    // Cycles through CLI options
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': infile = open(optarg, O_RDONLY); break;
        case 'o': outfile = open(optarg, O_RDWR | O_CREAT, 0666); break;
        case 'h': help = true; break;
        case 'v': stats = true; break;
        }
    }

    // Prints help message if user inputs incorrect CLI or the help option.
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

    // Cycles through entire file and constructs the histogram.
    do {
        size = read_bytes(infile, buf, BLOCK);
        for (int i = 0; i < size; i++) {
            hist[buf[i]]++;
        }

    } while (size != 0);

    // Adds 1 to beginning of historgram and end to help structure the
    // Huffman Tree
    hist[0]++;
    hist[255]++;

    // Constructs huffman tree.
    Node *root;
    root = build_tree(hist);

    // Builds code table.
    build_codes(root, table);

    // Constructs and writes the header.
    Header h;
    h.magic = MAGIC;
    h.permissions = s.st_mode;
    fchmod(outfile, s.st_mode);
    uint16_t tree_size = 0;
    for (uint16_t z = 0; z < 256; z++) {
        if (hist[z] > 0) {
            tree_size++;
        }
    }
    tree_size = tree_size * 3 - 1;
    h.tree_size = tree_size;
    h.file_size = s.st_size;
    write(outfile, &h, sizeof(h));

    // Dumps the Huffman Tree
    dump_tree(outfile, root);

    // Cycles through entire file from beginning again,
    // but this time writes the codes to outfile
    lseek(infile, 0, SEEK_SET);
    do {
        size = read_bytes(infile, buf, BLOCK);

        for (int i = 0; i < size; i++) {
            write_code(outfile, &table[buf[i]]);
        }

    } while (size != 0);
    flush_codes(outfile);

    // Credit: Reference program
    // Prints statistics if user asked.
    if (stats) {
        struct stat o;
        fstat(outfile, &o);
        printf("Uncompressed file size: %ld bytes\n", s.st_size);
        printf("Compressed file size: %ld bytes\n", o.st_size);
        printf(
            "Space saving: %.2f%% \n", 100.00 * (1.00 - ((float) o.st_size / (float) s.st_size)));
    }

    close(infile);
    close(outfile);
    return 0;
}

// Credit: Reference Program
// Prints the help manual.
void print_help(void) {
    printf("SYNOPSIS\n");
    printf("  Compress a file using the Huffman coding algorithm.\n");
    printf("\n");
    printf("USAGE\n");
    printf("  ./encode [-h] [-i infile] [-o outfile]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("  -h           Program usage and help.\n");
    printf("  -v           Prints compression statistics.\n");
    printf("  -i infile    Input file to compress.\n");
    printf("  -o outfile   Output of compressed data..\n");
}
