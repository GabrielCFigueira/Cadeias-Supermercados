/*
 * matrix.c
 * ASA 2018
 * Gabriel Figueira, Rafael Andrade
 * P1 (Sr. Joao Caracol)
 */

#include <stdio.h>
#include <stdlib.h>
#include "adj.h"

struct graph {
  int * data;
  int n_vertexes;
  int n_connections;
};


static void printCon(Graph g, int u, int v);

Graph buildGraph() {
  int N, M, vertex, edge;
  scanf("%d", &N);
  scanf("%d", &M);

  Graph res = (Graph) calloc(1, sizeof(struct graph));
  res->data = (int*) calloc((N+1)*(N+1), sizeof(int)); // Allocating +2N

  res->n_vertexes = N;
  res->n_connections = M;

  while(M--) {
    scanf("%d %d", &vertex, &edge);
    res->data[vertex*N + edge] = 1;
  }

  return res;

}

// To operate on a transposed graph, define the function that receives
// the same inputs that the normal
Graph  transposeGraph(Graph g) {
  return g;
}


void freeGraph(Graph g) {
  free(g->data);
  free(g);
}

void doForEachAdjU(Graph g, int u, void (*func)(Graph, int, int)) {
  int base = g->n_vertexes*u;
  for(int i=1; i <= g->n_vertexes; ++i) {
    if(g->data[base+i])
      func(g, u, i);
  }
}

int nVertex(Graph g) { return g->n_vertexes; } //TODO: Macro these
int nConnection(Graph g) { return g->n_connections; }

/* Shows that everything went fine (for debugging purposes)*/
void showGraph(const Graph g) {
  int base, i, j;
  for(i=1; i <= g->n_vertexes; ++i) {
    base=i*g->n_vertexes;
    printf("Vertex %d: ", i);
    for(j=1; j<= g->n_vertexes; ++j) {
      if(g->data[base + j]) {
        printf("%d ", j);
      }
    }
    putchar('\n');
  }
}

Graph reduceGraph(Graph g, int * translation) {

  int i, j, u, v, base;

  Graph res = (Graph) calloc(1, sizeof(struct graph));
  res->data = (int*) calloc((nVertex(g)+1)*(nVertex(g)+1), sizeof(int)); // Allocating +2N

  res->n_vertexes = nVertex(g);
  res->n_connections = 0;

  for (i = 1; i <= nVertex(g); i++) {
    base = g->n_vertexes*i;
    u = translation[i];
    for(j=1; j <= g->n_vertexes; ++j) {
      if(g->data[base+j]) {
        v = translation[j];
        if(u != v) {
          res->data[u*g->n_vertexes + v] = 1;
          res->n_connections++;
        }
      }
    }
  }

  return res;
}

void printSccGraph(Graph g, int nScc) {

  printf("%d\n%d\n", nScc, nConnection(g));
  for(int u=1; u<=nVertex(g); ++u)
    doForEachAdjU(g, u, printCon);

}


void printCon(Graph g, int u, int v) {
  (void) g;
  printf("%d %d\n", u, v);
}

