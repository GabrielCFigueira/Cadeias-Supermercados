#include <stdlib.h>
#include <limits.h>
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
static int stackPointer = 0;
static int * discovery;
static int * low;
static int * stack;

static void Tarjan(Graph g);
static void tarjanVisit(Graph g, int vertex);
static int min(int a, int b);
static int contains(int stack[], int vertex);


void Tarjan(Graph g) {

  int V = nVertex(g);
  stack = (int*) malloc(sizeof(int) * nConnection(g)); /*the stack can't go over the number of connections in the graph*/
  discovery = (int*) malloc(sizeof(int)*V);
  low = (int*) malloc(sizeof(int)*V);


  for (int i = 0; i < V; i++)
    discovery[i] = INT_MAX;
  for (int i = 0; i < V; i++)
    if (low[i] == INT_MAX)
      tarjanVisit(g, i);

}

void tarjanVisit(Graph g, int vertex) {

  discovery[vertex] = low[vertex] = visited++;
  stack[stackPointer++] = vertex;
  Node conn = getAdjList(g)[vertex];

  while(conn != NULL) {
    int adj = conn->id;
    if (low[adj] == INT_MAX || contains(stack, adj)) {
      if(low[adj] == INT_MAX)
        tarjanVisit(g, adj);
      low[vertex] = min(low[vertex], low[adj]);
    }
    conn = conn->next;
  }

  if(discovery[vertex] == low[vertex]) {
    int u = stack[stackPointer];
    while(stackPointer != 0 || u == vertex)
      /* POP(stack[stackPointer]); */
      stackPointer--;
  }


}

int min(int a, int b) {
  if(a > b)
    return a;
  else
    return b;
}

int contains(int * stack, int vertex) {
  int localStackPointer = stackPointer;
  for(; localStackPointer >= 0; localStackPointer--)
    if(stack[localStackPointer] == vertex)
      return 1;
  return 0;
}






int main() {
  Graph ola = buildGraph();
  showGraph(ola);
  DFS(ola);
  Tarjan(ola);
  freeGraph(ola);
  free(color+1);
  free(pai+1);
  free(f_time+1);
  free(d_time+1);
  return 0;
}
