/*
 * list.c
 * ASA 2018
 * Gabriel Figueira, Rafael Andrade
 * P1 (Sr. Joao Caracol)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /*for uint_fast types TODO*/







typedef struct node {
  struct node * next;
  int id;
} * Node;







/* creates a new node */
Node buildNode(int id) {

  Node new = (Node) calloc(1, sizeof(struct node));
  new->id = id;

  return new;

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

  while(M--) {
    scanf("%d %d", &vertex, &edge);
    insertInAdjList(&adjList[vertex], buildNode(edge));
  }

  return adjList;
}
