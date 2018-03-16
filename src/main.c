#include <stdlib.h>
#include <limits.h>
#include <string.h>
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




static int visited = 0;
static int n_scc = 0;
static int stackPointer = -1; //Points to the first element that exists
static int * discovery;
static int * low;
static int * translation;
static int * scratch; static int scratchPointer=0;
static int * stack;
static int * in_stack;

static void Tarjan(Graph g);
static void tarjanVisit(Graph g, int vertex);


#define min(a,b) (a<b?a:b)

#define stack_contains(a) (in_stack[a])

void Tarjan(Graph g) {
  //because we really want to go [1, nVertex], we do [0, nVertex]
  int V = nVertex(g) + 1;
  int E = nConnection(g);
  // the stack can't go over the number of connections in the graph
  stack = (int*) calloc(E, sizeof(int));
  in_stack = (int*) calloc(V, sizeof(int));
  scratch = (int*) calloc(E, sizeof(int));
  translation = (int*) calloc(V, sizeof(int));
  discovery = (int*) malloc(sizeof(int)*V);
  low = (int*) malloc(sizeof(int)*V);

  int i;
  for(i=V; i>=0; --i)
    low[i] = INT_MAX;
  for(i=V; i>=0; --i)
    discovery[i] = INT_MAX;

  for (int i = 1; i < V; i++)
    if (low[i] == INT_MAX)
      tarjanVisit(g, i);

}


void tarjanVisit_aux(Graph g, int from, int to) {

  if (low[to] == INT_MAX || stack_contains(to)) {
      if(low[to] == INT_MAX) {
        tarjanVisit(g, to);
      }
      low[from]=min(low[from],low[to]);
  }
}

void tarjanVisit(Graph g, int vertex) {

  int v;
  discovery[vertex] = low[vertex] = visited++;
  stack[++stackPointer] = vertex; //Push
  in_stack[vertex] = 1;

  doForEachAdjU(g, vertex, tarjanVisit_aux);

  if(discovery[vertex] == low[vertex]) {
    int lowest_in_scc = vertex;
    n_scc++;
    while(vertex != (v = stack[stackPointer--])) {
      in_stack[v] = 0;
      lowest_in_scc = min(v,lowest_in_scc);
      scratch[++scratchPointer] = v;
    }
    scratch[++scratchPointer] = vertex;
    while(0 != (v=scratch[scratchPointer--])) {
      translation[v] = lowest_in_scc;
    }
  }
}





int main() {
  Graph ola = buildGraph();
  showGraph(ola);
  Tarjan(ola);
  /*DFS(ola);
  freeGraph(ola);
  free(color+1);
  free(pai+1);
  free(f_time+1);
  free(d_time+1);*/
  return 0;
}
