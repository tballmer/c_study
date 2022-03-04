#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "hvui:o:"

// Credit for the program goes to Christan, Omar, and Eugene.
// Specific lines will have their own credits.

void print_help(void);
void dfs(Graph *G, uint32_t v, Path *curr, Path *shortest, char *cities[], FILE *outfile,
    bool verbose, uint32_t *calls);

int main(int argc, char **argv) {
    FILE *infile = stdin; // Credit for infile and outfile control: Eugene.
    FILE *outfile = stdout;
    bool help = false;
    bool verbose = false;
    bool undirected = false;
    char buffer[1024];
    unsigned vertices = 0;
    unsigned i = 0;
    unsigned j = 0;
    unsigned k = 0;
    int opt = 0;
    uint32_t calls = 0;

    // Credit for infile and outfile implementation using fopen(): Eugene.
    // Cycles through CLI options.
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'h': help = true; break;
        case 'v': verbose = true; break;
        case 'u': undirected = true; break;
        }
    }

    // Checks if input or output file is nonexistent.
    if (infile == false && outfile == false) {
        printf("Error: failed to open infile.\n");
        exit(0);
    }
    if (infile == false && outfile) {
        printf("Error: failed to open infile.\n");
        fclose(outfile);
        exit(0);
    }
    if (infile && outfile == false) {
        printf("Error: failed to open outfile.\n");
        fclose(infile);
        exit(0);
    }

    // If tsp is given incorrect inputs or help is enabled, the
    // synopsis page is printed.
    if (optopt || help) {
        print_help();
        // Christian explained to me how to use fclose().
        fclose(infile);
        fclose(outfile);
        exit(0);
    }

    // Credit for how to input using buffer, ffgets and sscanf: Eugene and Omar.
    // Takes the input for the number of vertices and handles errors.
    fgets(buffer, 1024, infile);
    if (sscanf(buffer, "%u", &vertices) != 1 || vertices > VERTICES) {
        fprintf(outfile, "Error: malformed number of vertices.\n");
        fclose(infile);
        fclose(outfile);
        exit(0);
    } else if (vertices == 1) {
        fprintf(outfile, "There's nowhere to go.");
    }

    // Credit for how to take user string input and save in and array:
    // Eugene and Omar.
    // Initialzes an array of pointers to strings, then copies input into
    // the cities array.
    char *cities[vertices];
    for (unsigned i = 0; i < vertices; i++) {
        fgets(buffer, 1024, infile);
        buffer[strlen(buffer) - 1] = '\0';
        cities[i] = strdup(buffer);
    }

    // Credit for proper use of sscanf: Omar.
    // Creates a graph and takes edge input as a triple.
    Graph *G = graph_create(vertices, undirected);
    while (fgets(buffer, 1024, infile) != NULL) {
        if (sscanf(buffer, "%u %u %u", &i, &j, &k) != 3) {
            fprintf(outfile, "Error: malformed edge.\n");
            graph_delete(&G);
            fclose(infile);
            fclose(outfile);
            for (unsigned x = 0; x < vertices; x++) {
                free(cities[x]);
                cities[x] = NULL;
            }
            exit(0);
        }
        graph_add_edge(G, i, j, k);
    }

    // Creates a current path and shortest path for use in dfs().s
    Path *curr = path_create();
    Path *shortest = path_create();

    // Runs dfs search and prints results.
    dfs(G, START_VERTEX, curr, shortest, cities, outfile, verbose, &calls);
    fprintf(outfile, "Path length: %" PRIu32 "\n", path_length(shortest));
    fprintf(outfile, "Path: %s -> ", cities[0]);
    path_print(shortest, outfile, cities);
    fprintf(outfile, "Total recursive calls: %d\n", calls);

    // Deletes paths, and graph. Closes files and frees cities.
    // This prevents memory leaks.
    path_delete(&curr);
    path_delete(&shortest);
    graph_delete(&G);
    fclose(infile);
    fclose(outfile);
    for (unsigned x = 0; x < vertices; x++) {
        free(cities[x]);
        cities[x] = NULL;
    }
    return 0;
}

// The main structure for dfs() is taken from the assignment document.
// DFS recursively searches through every possible Hamiltonian path.
void dfs(Graph *G, uint32_t v, Path *curr, Path *shortest, char *cities[], FILE *outfile,
    bool verbose, uint32_t *calls) {
    uint32_t pop_value = 0;
    *calls += 1;
    graph_mark_visited(G, v);
    for (uint32_t w = 0; w < VERTICES; w++) { // Loops through matrix to find an edge.
        if (graph_has_edge(G, v, w)) { // Only continues if edge is found.
            path_push_vertex(curr, w, G);
            // Only continues if curr is shorter than the shortest or
            // if this is the beginning of the search.
            if (w == 0 || path_length(shortest) == 0 || path_length(curr) < path_length(shortest)) {
                // Checks if this is a hamiltonian path.
                if (w == 0 && path_vertices(curr) == graph_vertices(G)) {
                    if (verbose) {
                        fprintf(outfile, "Path length: %" PRIu32 "\n", path_length(curr));
                        fprintf(outfile, "Path: %s -> ", cities[0]);
                        path_print(curr, outfile, cities);
                    }
                    // Makes sure that the path to copy is the shortest or
                    // if this is the beginning of the search.
                    if (path_length(shortest) == 0 || path_length(curr) < path_length(shortest)) {
                        path_copy(shortest, curr);
                    }
                    // If this is not the end of the path recursively call dfs().
                } else if (graph_visited(G, w) == false) {
                    dfs(G, w, curr, shortest, cities, outfile, verbose, calls);
                }
            }
            path_pop_vertex(curr, &pop_value, G);
        }
    }
    graph_mark_unvisited(G, v);
    return;
}

// Credit for synopsis printout: reference binary.
// Prints out the help menu.
void print_help(void) {
    printf("SYNOPSIS\n");
    printf("  Hamiltonian Path Traveling Salesman Problem using DFS.\n");
    printf("\n");
    printf("USAGE\n");
    printf("  ./tsp [-u] [-v] [-h] [-i infile] [-o outfile]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("  -h           Program usage and help.\n");
    printf("  -i infile    Input containing graph (default: stdin).\n");
    printf("  -o outfile   Output of computed path (default: stdout).\n");
    printf("  -u           Use undirected graph.\n");
    printf("  -v           Enable verbose printing.\n");
}
