/*
 * mooshak.c
 * ASA 2018
 * Gabriel Figueira, Rafael Andrade
 * P1 (Sr. Joao Caracol)
 */




#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>





/*-------------------------------------------------------------------------*/
/*------------------------Graph representation-----------------------------*/
/*-------------------------------------------------------------------------*/



typedef struct graph * Graph;
struct graph {
  int n_vertexes;
  int n_connections;
  int (*adjList)[2];
  int *offset;
} ;

#define nVertex(g) (g->n_vertexes)
#define nConnection(g) (g->n_connections)


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



/*function which reads from input and builds a Graph */
Graph buildGraph() {

  int V, E, u, v, i;
  scanf("%d", &V); /* number of vertexes */
  scanf("%d", &E); /* number of connections */

  Graph res = (Graph) calloc(1, sizeof(struct graph));
  res->offset = (int*) calloc(V+1, sizeof(int));
  res->adjList = malloc(E* sizeof(*res->adjList));

  res->n_vertexes = V;
  res->n_connections = E;

  while(E--) { /*reads every connection */
    scanf("%d %d", &u, &v);
    res->adjList[E][0]=u; res->adjList[E][1]=v;
    res->offset[u]++;
  }

  /*sorts the connections read*/
  graphSort(res->adjList, res->offset, res->n_connections, res->n_vertexes);
  for(i=1; i<=V; ++i) {
    res->offset[i]+=res->offset[i-1];
  }

  return res;
}

/*destroys the graph*/
void freeGraph(Graph g) {
  free(g->adjList);
  free(g->offset);
  free(g);
}

/* applies the function func to all vertexes adjacent to vertex u */
void doForEachAdjU(Graph g, int u, void (*func)(Graph, int, int)) {
  int base, max;
  base=g->offset[u-1];
  max=g->offset[u];
  while(base < max) {
    func(g, u, g->adjList[base][1]);
    base++;
  }
}


/* removes redundant connections and reduces the Graph to its SCCs */
Graph reduceGraph(Graph g, int * translation) {

  int u, v, n_conns=0, i, j, old_u, old_v;
  Graph res = (Graph) calloc(1, sizeof(struct graph));
  res->offset = (int*) calloc(nVertex(g)+1, sizeof(int));
  res->adjList = malloc(nConnection(g)* sizeof(*res->adjList));

  res->n_vertexes = nVertex(g);


  /* translates the vertexes to the minimum in its respective SCC, and removes
     duplicates */
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

  /* removes redudant connections */
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

/*prints the Graph, shwoing the number of SCCs, connections intra-SCC and
each connection */
void printSccGraph(Graph g, int nScc) {
  int u;
  printf("%d\n%d\n", nScc, nConnection(g));
  for(u=0; u<nConnection(g); ++u) {
    printf("%d %d\n", g->adjList[u][0], g->adjList[u][1]);
  }
}




/*-------------------------------------------------------------------------*/
/*------------------------Tarjan algorithm---------------------------------*/
/*-------------------------------------------------------------------------*/
static int visited = 0;
static int n_scc = 0;
static int stackPointer = -1;
static int * discovery;
static int * low;
static int * translation;
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

  discovery[vertex] = low[vertex] = visited++;
  stack[++stackPointer] = vertex; /*Push*/
  in_stack[vertex] = 1;

  doForEachAdjU(g, vertex, tarjanVisit_aux);


  if(discovery[vertex] == low[vertex]) {
    int v, lowest_in_scc, sccStackTop;
    sccStackTop=stackPointer;
    lowest_in_scc = vertex;
    n_scc++;
    /* get the lowest identifier in SCC */
    while(vertex != (v = stack[stackPointer--])) { /*Pop*/
      in_stack[v] = 0;
      lowest_in_scc = min(v,lowest_in_scc);
    }
    in_stack[vertex] = 0;
    while(stackPointer != sccStackTop) {
      translation[stack[sccStackTop]] = lowest_in_scc;
      sccStackTop--;
    }
  }
}

/* frees the memory structures associated with the Tarjan algorithm*/
void freeTarjan() {
  free(stack);
  free(in_stack);
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
