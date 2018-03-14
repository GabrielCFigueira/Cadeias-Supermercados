/*
 * list.c
 * ASA 2018
 * Gabriel Figueira, Rafael Andrade
 * P1 (Sr. Joao Caracol)
 */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct node {
  struct node * next;
  int id;
} * Node;

static int N_VERTEXES;
static int N_CONNECTIONS;
static Node * adjListAuxPointer;


static Node buildNode(int id);
static void freeNode(int from, Node to);
static void invertConnection(int from, Node to);
static void insertInAdjList(Node * adjList, Node new);



/* creates a new node */
Node buildNode(int id) {

  Node new = (Node) calloc(1, sizeof(struct node));
  new->id = id;

  return new;
}

/* frees memory associated with a node*/
void freeNode(int i, Node n) {
  (void) i;
  free(n);
}



/* inserts a new node at the beggining of the adjacency list */
void insertInAdjList(Node * adjList, Node new) {

  if(*adjList != NULL) {
    new->next = (*adjList)->next;
    (*adjList)->next = new;
  }

  *adjList = new;

}



/* funtion which receives input and builds the adjacency list*/
Node * buildAdjList() {
  int N, M, vertex, edge;
  scanf("%d", &N);
  scanf("%d", &M);
  Node * adjList = (Node*) calloc(1, sizeof(Node) * N);

  adjList--; /*the vertexes are bounded from 1 to N*/

  N_VERTEXES = N;
  N_CONNECTIONS = M;

  while(M--) {
    scanf("%d %d", &vertex, &edge);
    insertInAdjList(&adjList[vertex], buildNode(edge));
  }

  return adjList;
}


/* Meta function to do something with the nodes in the adjency list
 * It goes in order of vertexes */
void traverseAdjList(Node * adjList, void (*func)(int, Node)){
  Node conn, scratchpad;
  for(int i=1; i <= N_VERTEXES; ++i) {
    conn = adjList[i];
    while(conn != NULL) {
      scratchpad = conn;
      conn = conn->next;
      func(i, scratchpad);
    }
  }
}

/* Insert (inverted) connection in new adjacency list auxiliary pointer */
void invertConnection(int from, Node to) {
  insertInAdjList(&adjListAuxPointer[to->id], buildNode(from));
}


/* Transposes list of adjacencies, one node at a time */
Node * transposeAdjList(Node * adjList) {

  adjListAuxPointer = (Node*) calloc(1, sizeof(Node) * N_VERTEXES);
  adjListAuxPointer--;
  traverseAdjList(adjList, invertConnection);
  return adjListAuxPointer;

}


/* Shows that everything went fine (for debugging purposes)*/
void showAdjList(const Node * adjList) {
  Node conn;
  for(int i=1; i <= N_VERTEXES; ++i) {
    printf("Vertex %d: ", i);
    conn = adjList[i];
    while(conn != NULL) {
      printf("%d ", conn->id);
      conn = conn->next;
    }
    putchar('\n');
  }
}


void freeAdjList(Node * adjList) {

  traverseAdjList(adjList, freeNode);
  free(++adjList);

}
