#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include "adj.h"


static int visited = 0;
static int n_scc = 0;
/* Points to the first element that exists */
static int stackPointer = -1;
static int * discovery;
static int * low;
static int * translation;
/*static int * scratch; */
static int SCCPointer=0;
static int * stack;
static int * in_stack;

static void Tarjan(Graph g);
static void tarjanVisit(Graph g, int vertex);
static void tarjanVisit_aux(Graph g, int from, int to);
static void freeTarjan();

#define min(a,b) (a<b?a:b)
#define stack_contains(a) (in_stack[a])

void Tarjan(Graph g) {
  /* because we really want to go [1, nVertex], we do [0, nVertex] */
  int V = nVertex(g) + 1;
  /* the stack can't go over the number of vertexes in the graph */
  stack = (int*) calloc(V, sizeof(int));
  in_stack = (int*) calloc(V, sizeof(int));
  /*scratch = (int*) calloc(V, sizeof(int));*/
  translation = (int*) calloc(V, sizeof(int));
  discovery = (int*) malloc(sizeof(int)*V);
  low = (int*) malloc(sizeof(int)*V);

  int i;
  for(i=1; i<V; i++)
    low[i] = INT_MAX;
  for(i=1; i<V; i++)
    discovery[i] = INT_MAX;

  for(i = 1; i < V; i++)
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
  stack[++stackPointer] = vertex; /*Push*/
  ++SCCPointer;
  in_stack[vertex] = 1;

  doForEachAdjU(g, vertex, tarjanVisit_aux);

  if(discovery[vertex] == low[vertex]) {
    int lowest_in_scc = vertex;
    n_scc++;
    while(vertex != (v = stack[stackPointer--])) { /*Pop*/
      in_stack[v] = 0;
      lowest_in_scc = min(v,lowest_in_scc);
    }
    in_stack[vertex] = 0;
    while(stackPointer != SCCPointer) {
      translation[stack[SCCPointer]] = lowest_in_scc;
      SCCPointer--;
    }
  }
}


void freeTarjan() {
  free(stack);
  free(in_stack);
  /*free(scratch);*/
  free(translation);
  free(discovery);
  free(low);
}


int main() {
  Graph initial, reduced;
  initial = buildGraph();
  Tarjan(initial);
  reduced = reduceGraph(initial, translation);
  printSccGraph(reduced, n_scc);
  freeGraph(reduced);
  freeTarjan();
  return 0;
}
