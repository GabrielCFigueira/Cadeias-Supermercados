/*
 * list.c
 * ASA 2018
 * Gabriel Figueira, Rafael Andrade
 * P1 (Sr. Joao Caracol)
 */

#include <stdio.h>
#include <stdlib.h>
#include "adj.h"

struct graph {
  int n_vertexes;
  int n_connections;
  int (*adjList)[2];
  int *offset;
} ;

static int compareCon(const void *a, const void *b);
static void printCon(Graph g, int u, int v);

int compareCon(const void *a, const void *b) {
  int *first_edge = (int*) a;
  int *sec_edge = (int*) b;
  if (first_edge[0] < sec_edge[0]) {
    return -1;
  }
  if(first_edge[0] > sec_edge[0]) {
    return 1;
  }
  if (first_edge[1] < sec_edge[1]) {
    return -1;
  }
  if(first_edge[1] > sec_edge[1]) {
    return 1;
  }
  return 0;

}


Graph buildGraph() {
  int V, E, u, v, i;
  scanf("%d", &V);
  scanf("%d", &E);

  Graph res = (Graph) calloc(1, sizeof(struct graph));
  res->offset = (int*) calloc(V+1, sizeof(int));
  res->adjList = malloc(E* sizeof(*res->adjList));

  res->n_vertexes = V;
  res->n_connections = E;

  while(E--) {
    scanf("%d %d", &u, &v);
    res->adjList[E][0]=u; res->adjList[E][1]=v;
    res->offset[u]++;
  }

  /*Do a couting sort (implement it)*/
  qsort(res->adjList, res->n_connections, 2*sizeof(int), compareCon);
  for(i=1; i<=V; ++i) {
    res->offset[i]+=res->offset[i-1];
  }

  return res;
}

Graph transposeGraph(Graph g) { return g; } /*TODO*/

void showGraph(const Graph g) {
  int base, max, u;
  max = 0;
  for(u=1; u <= g->n_vertexes; ++u) {
    base=max;
    max=g->offset[u];
    printf("Vertex %d: ", u);
    while(base < max) {
        printf("%d ", g->adjList[base][1]);
        base++;
      }
    putchar('\n');
    }
}

void freeGraph(Graph g) {
  free(g->adjList);
  free(g->offset);
  free(g);
}

void doForEachAdjU(Graph g, int u, void (*func)(Graph, int, int)) {
  int base, max;
  base=g->offset[u-1];
  max=g->offset[u];
  while(base < max) {
    func(g, u, g->adjList[base][1]);
    base++;
  }
}


int nVertex(Graph g) { return g->n_vertexes; }
int nConnection(Graph g) { return g->n_connections; }


Graph reduceGraph(Graph g, int * translation) {

  int u, v, n_conns=0, i, j, old_u, old_v;
  Graph res = (Graph) calloc(1, sizeof(struct graph));
  res->offset = (int*) calloc(nVertex(g)+1, sizeof(int));
  res->adjList = malloc(nConnection(g)* sizeof(*res->adjList));

  res->n_vertexes = nVertex(g);

  j=0;
  for (i = 0; i < nConnection(g); i++) {
    u=translation[g->adjList[i][0]];
    v=translation[g->adjList[i][1]];
    if(u != v) {
      g->adjList[j][0]=u; g->adjList[j][1]=v;
      res->offset[u]++; j++;
    }
  }

  /* Do a couting sort (implement it) */
  qsort(g->adjList, j, 2*sizeof(int), compareCon);

  old_u = old_v = 0;
  for(i=0; i < j; ++i) {
    u=g->adjList[i][0];
    v=g->adjList[i][1];
    if(v!=old_v || u!=old_u) {
      res->adjList[n_conns][0]=u;
      res->adjList[n_conns][1]=v;
      old_u=u;
      old_v=v;
      n_conns++;
    } else {
      res->offset[u]--;
    }
  }


  res->n_connections=n_conns;

  for(i=1; i<=nVertex(g); ++i) {
    res->offset[i]+=res->offset[i-1];
  }

  return res;
}

void printSccGraph(Graph g, int nScc) {
  int u;
  printf("%d\n%d\n", nScc, nConnection(g));
  for(u=1; u<=nVertex(g); ++u)
    doForEachAdjU(g, u, printCon);

}


void printCon(Graph g, int u, int v) {
  (void) g;
  printf("%d %d\n", u, v);
}
