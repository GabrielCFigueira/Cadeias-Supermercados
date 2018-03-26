/*
 * fast_list.c
 * ASA 2018
 * Gabriel Figueira, Rafael Andrade
 * P1 (Sr. Joao Caracol)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adj.h"

struct graph {
  int n_vertexes;
  int n_connections;
  int (*adjList)[2];
  int *offset;
} ;

static void graphSort(int (*adjList)[2], int *count, int n_conns, int n_vertexes);

/* sorts a 2xn_conns array, with the first line being the most significant
 * for sorting purposes
 * this sort used radixSort (lsd), and couting sort to sort each array
 */
void graphSort(int (*adjList)[2], int *count, int n_conns, int n_vertexes) {


  /* offset goes [0, V]*/
  int * offset = (int*) calloc (1, (n_vertexes + 1)*sizeof(int));
  /* goes [0, E-1]*/
  int (*newAdj)[2] = malloc(n_conns*sizeof(*newAdj));

  int i;

  for(i = 0; i < n_conns; i++)
    offset[adjList[i][1]]++;

  for(i = 1; i <= n_vertexes; i++)
    offset[i] += offset[i - 1];

  --offset;
  for(i = 0; i < n_conns; i++) {
    newAdj[offset[adjList[i][1]]][1] = adjList[i][1];
    newAdj[offset[adjList[i][1]]][0] = adjList[i][0];
    offset[adjList[i][1]]++;
  }
  ++offset;


  memcpy(offset, count, (n_vertexes+1)*sizeof(int));

  for(i = 1; i <= n_vertexes; i++)
    offset[i] += offset[i - 1];

  --offset;
  for(i = 0; i < n_conns; i++) {
    adjList[offset[newAdj[i][0]]][0] = newAdj[i][0];
    adjList[offset[newAdj[i][0]]][1] = newAdj[i][1];
    offset[newAdj[i][0]]++;
  }
  ++offset;

  free(offset);
  free(newAdj);
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

  graphSort(res->adjList, res->offset, res->n_connections, res->n_vertexes);
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

  graphSort(g->adjList, res->offset, j, res->n_vertexes);

  old_u = old_v = 0;
  for(i=0; i < j; ++i) {
    u = g->adjList[i][0];
    v = g->adjList[i][1];
    if(old_v != v || old_u != u) {
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

  freeGraph(g);
  return res;
}

void printSccGraph(Graph g, int nScc) {
  int u;
  printf("%d\n%d\n", nScc, nConnection(g));
  for(u=0; u<nConnection(g); ++u) {
    printf("%d %d\n", g->adjList[u][0], g->adjList[u][1]);
  }
}
