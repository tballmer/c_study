#include "graph.h"

#include "vertices.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Graph structure is taken from the assignment document.
struct Graph {
    uint32_t vertices; // Number of vertices.
    bool undirected; // Undirected graph?
    bool visited[VERTICES]; // Where have we gone?
    uint32_t matrix[VERTICES][VERTICES]; // Adjacency matrix.
};

// graph_create() is taken from the assignment document.
// Constructs the graph and dynamically allocates memory.
Graph *graph_create(uint32_t vertices, bool undirected) {
    Graph *G = (Graph *) calloc(1, sizeof(Graph));
    G->vertices = vertices;
    G->undirected = undirected;
    return G;
}

// graph_delete() is taken from the assignment document.
// Deletes graph by freeing allocated memory.
void graph_delete(Graph **G) {
    free(*G);
    *G = NULL; // Prevents G pointer from being used after being freed.
    return;
}

// Returns number of vertices.
uint32_t graph_vertices(Graph *G) {
    return G->vertices;
}

// Adds an edge and edgeweight to the matrix with error checks.
bool graph_add_edge(Graph *G, uint32_t i, uint32_t j, uint32_t k) {
    if (i < G->vertices && j < G->vertices && i != j) {
        if (G->undirected) { // Checks if the graph is undirected.
            G->matrix[j][i] = k;
        }
        G->matrix[i][j] = k;
        return true;
    }
    return false;
}

// Checks if graph has an edge between two vertices with error checks.
bool graph_has_edge(Graph *G, uint32_t i, uint32_t j) {
    if (i < G->vertices && j < G->vertices && G->matrix[i][j] > 0) {
        return true;
    }
    return false;
}

// Returns the edge weight between two vertices.
uint32_t graph_edge_weight(Graph *G, uint32_t i, uint32_t j) {
    if (i < G->vertices && j < G->vertices && G->matrix[i][j] > 0) {
        return G->matrix[i][j];
    }
    return 0; // If there is no edge, 0 is returned.
}

// Checks if a vertex was visited.
bool graph_visited(Graph *G, uint32_t v) {
    if (v < G->vertices) {
        return G->visited[v];
    }
    return false;
}

// Marks a vertex as visited.
void graph_mark_visited(Graph *G, uint32_t v) {
    if (v < G->vertices) {
        G->visited[v] = true;
    }
}

// Marks a vertex as unvisited.
void graph_mark_unvisited(Graph *G, uint32_t v) {
    if (v < G->vertices) {
        G->visited[v] = false;
    }
}

// Debugging function which prints out entire matrix.
// Useful to view all edges of a graph.
void graph_print(Graph *G) {
    for (uint32_t row = 0; row < VERTICES; row++) {
        for (uint32_t col = 0; col < VERTICES; col++) {
            printf("%" PRIu32 " ", G->matrix[row][col]);
            if (col == VERTICES - 1) {
                printf("\n");
            }
        }
    }
}
