#include "path.h"

#include "graph.h"
#include "stack.h"
#include "vertices.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Path structure is taken from the assignment document.
struct Path {
    Stack *vertices; // Path is a wrapper around Stack
    uint32_t length;
};

// path_create() is the constructor for the path.
Path *path_create(void) {
    Path *p = (Path *) malloc(sizeof(Path));
    p->vertices = stack_create(VERTICES);
    p->length = 0;
    return p;
}

// path_delete() is the destructor for the path.
void path_delete(Path **p) {
    stack_delete(&((*p)->vertices));
    free(*p);
    *p = NULL;
    return;
}

// Pushes a vertex onto the the path.
bool path_push_vertex(Path *p, uint32_t v, Graph *G) {
    uint32_t peek = 0;
    stack_peek(p->vertices, &peek);
    // Error handling.
    if (stack_push(p->vertices, v) == false) {
        return false;
    }
    p->length += graph_edge_weight(G, peek, v);
    return true;
}

// Pops a vertex off of the path.
bool path_pop_vertex(Path *p, uint32_t *v, Graph *G) {
    uint32_t peek = 0;
    // Error handling.
    if (stack_pop(p->vertices, v) == false) {
        return false;
    }
    stack_peek(p->vertices, &peek);
    p->length -= graph_edge_weight(G, peek, *v);
    return true;
}

// Returns the number of vertices in the path.
uint32_t path_vertices(Path *p) {
    return stack_size(p->vertices);
}

// Returns the length of the path which is calculated
// by adding the weight of all edges in the path.
uint32_t path_length(Path *p) {
    return p->length;
}

// Copies the path src into the path dst,
// assumes that dst is properly initialized.
void path_copy(Path *dst, Path *src) {
    stack_copy(dst->vertices, src->vertices);
    dst->length = src->length;
}

// Prints the specified path.
void path_print(Path *p, FILE *outfile, char *cities[]) {
    stack_print(p->vertices, outfile, cities);
}
