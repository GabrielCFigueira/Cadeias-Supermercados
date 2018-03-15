#include <stdlib.h>
#include "adj.h"

static int * color;
static int * pai;
static int * d_time;
static int * f_time;
static int clock = 1;

void DFS_visit(Graph f, int u);
void DFS_visit_aux(Graph g, int u, int v);
void DFS(Graph g);

void DFS_visit(Graph g, int u) {
  color[u] = -1;
  d_time[u] = clock++;
  doForEachAdjU(g,u,DFS_visit_aux);
  color[u] = 1;
  f_time[u] = clock++;
}


void DFS_visit_aux(Graph g, int u, int v) {
  if(!color[v]) {
    pai[v] = u;
    DFS_visit(g, v);
  }
}


void DFS(Graph g) {
  int V = nVertex(g);
  color = (int*) calloc(V, sizeof(int)) - 1;
  pai = (int*) calloc(V, sizeof(int)) - 1;
  d_time = (int*) calloc(V, sizeof(int)) - 1;
  f_time = (int*) calloc(V, sizeof(int)) - 1;
  for(int u = 1; u<=V; ++u) {
    if(!color[u]) {
      DFS_visit(g, u);
    }
  }
}

int main() {
  Graph ola = buildGraph();
  showGraph(ola);
  DFS(ola);
  freeGraph(ola);
  free(color+1);
  free(pai+1);
  free(f_time+1);
  free(d_time+1);
  return 0;
}
