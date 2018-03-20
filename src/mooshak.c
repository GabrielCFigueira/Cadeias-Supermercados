#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

/*fast_list.c*/

typedef struct graph * Graph;
struct graph {
  int n_vertexes;
  int n_connections;
  int (*adjList)[2];
  int *offset;
} ;

#define nVertex(g) (g->n_vertexes)
#define nConnection(g) (g->n_connections)



static int compare2Con(const void *a, const void *b);
static int (*graphSort(int (*adjList)[2], int *count, int n_conns, int n_vertex))[2];

int (*graphSort(int (*adjList)[2], int *count, int n_conns, int n_vertexes))[2] {

  int i, pos, u;
  int (*newAdj)[2] = malloc(n_conns*sizeof(*newAdj));
  /*This is an offset list with an extra position (+2 vs +1) so that
   * we can have a -1 position of 0, because, during the positioning,
   * the offsets will shift to the left, and so we will use that extra space
   * to go back to a normal offset list.*/
  int * aux_count = (int*) malloc((n_vertexes+2)*sizeof(int));
  aux_count[0]=0;
  aux_count++;
  memcpy(aux_count, count, (n_vertexes+1)*sizeof(int));

  for(i=1; i<=n_vertexes; ++i) {
    aux_count[i]+=aux_count[i-1];
  }

  for(i=0; i<n_conns; ++i) {
    u=adjList[i][0];
    pos=aux_count[u-1]++;
    newAdj[pos][0] = u;
    newAdj[pos][1] = adjList[i][1];
  }
  aux_count--;

  for(i=1; i<=n_vertexes; ++i) {
    if(count[i] > 1) {
      qsort(newAdj+aux_count[i-1],count[i],2*sizeof(int),compare2Con);
    }
  }
  free(adjList);
  free(aux_count);
  return newAdj;
}

int compare2Con(const void *a, const void *b) {
  int *first_edge = (int*) a;
  int *sec_edge = (int*) b;

  return first_edge[1] - sec_edge[1];

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

  res->adjList = graphSort(res->adjList, res->offset, res->n_connections, res->n_vertexes);
  for(i=1; i<=V; ++i) {
    res->offset[i]+=res->offset[i-1];
  }

  return res;
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

  g->adjList = graphSort(g->adjList, res->offset, j, res->n_vertexes);

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





/*Tarjan algorithm*/

static int visited = 0;
static int n_scc = 0;
/* Points to the first element that exists */
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
