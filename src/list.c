/*
 * list.c
 * ASA 2018
 * Gabriel Figueira, Rafael Andrade
 * P1 (Sr. Joao Caracol)
 */

#include <stdio.h>
#include <stdlib.h>
#include "adj.h"



typedef struct node {
  struct node * next;
  int id;
} * Node;


struct graph {
  Node * adjList;
  int n_vertexes;
  int n_connections;
} ;

static Node * adjListAuxPointer;

static void freeNode(int from, Node to);
static void traverseGraph(Graph g, void (*func)(int, Node));
static void invertConnection(int from, Node to);
static void insertInAdjList(Node * adjList, int id);


int nVertex(Graph g) { return g->n_vertexes; }
int nConnection(Graph g) { return g->n_connections; }


/* frees memory associated with a node*/
void freeNode(int i, Node n) {
  (void) i;
  free(n);
}


/* inserts a new node at the beggining of the adjacency list */
void insertInAdjList(Node * adjList, int id) {

  Node new = (Node) calloc(1, sizeof(struct node));
  new->id = id;
  new->next = *adjList;
  *adjList = new;
}



/* function which receives input and builds the adjacency list*/
Graph readGraph() {
  int N, M, vertex, edge;
  scanf("%d", &N);
  scanf("%d", &M);

  Graph res = buildGraph(N);

  res->n_connections = M;

  while(M--) {
    scanf("%d %d", &vertex, &edge);
    insertInAdjList(res->adjList + vertex, edge);
  }

  return res;
}


Graph buildGraph(int N) {
  
  Graph res = (Graph) calloc(1, sizeof(struct graph));
  res->adjList = (Node*) calloc(1, sizeof(Node) * N);

  res->adjList--; /*the vertexes are bounded from 1 to N*/

  res->n_vertexes = N;
  res->n_connections = 0;

  return res;
}

/* Meta function to do something with the nodes in the adjacency list
 * It goes in order of vertexes */
void traverseGraph(Graph g, void (*func)(int, Node)){
  Node conn, scratchpad;
  for(int i=1; i <= g->n_vertexes; ++i) {
    conn = g->adjList[i];
    while(conn != NULL) {
      scratchpad = conn;
      conn = conn->next;
      func(i, scratchpad);
    }
  }
}

/* Insert (inverted) connection in new adjacency list auxiliary pointer */
void invertConnection(int from, Node to) {
  insertInAdjList(adjListAuxPointer + to->id, from);
}


/* Transposes list of adjacencies, one node at a time */
Graph transposeGraph(const Graph g) {

  adjListAuxPointer = (Node*) calloc(1, sizeof(Node) * g->n_vertexes);
  adjListAuxPointer--;
  traverseGraph(g, invertConnection);
  Graph res = (Graph) calloc(1, sizeof(struct graph));
  res->n_vertexes = g->n_vertexes;
  res->n_connections = g->n_connections;
  res->adjList = adjListAuxPointer;
  return res;

}


/* Shows that everything went fine (for debugging purposes)*/
void showGraph(const Graph g) {
  Node conn;
  for(int i=1; i <= g->n_vertexes; ++i) {
    printf("Vertex %d: ", i);
    conn = g->adjList[i];
    while(conn != NULL) {
      printf("%d ", conn->id);
      conn = conn->next;
    }
    putchar('\n');
  }
}



void freeGraph(Graph g) {
  traverseGraph(g, freeNode);
  free(++(g->adjList));
  free(g);
}

void doForEachAdjU(Graph g, int u, void (*func)(Graph, int, int)) {
  Node conn, scratchpad;
  conn = g->adjList[u];
  while(conn != NULL) {
    scratchpad = conn;
    conn = conn->next;
    func(g, u, scratchpad->id);
  }
}
