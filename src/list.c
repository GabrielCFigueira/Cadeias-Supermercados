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







/* finds the first non null pointer and inserts it in the adjacency list */
void insertInAdjList(Node orig, int id) {

  if(orig != NULL)
    while(orig->next != NULL)
      orig = orig->next;

  orig = (Node) calloc(1, sizeof(struct node));
  orig->id = id;


}




Node * buildAdjList() {
  int N, M, vertex, edge;
  scanf("%d", &N);
  scanf("%d", &M);
  Node * adjList = (Node*) calloc(1, sizeof(Node) * N);
  adjList--;
  while(M--) {
    scanf("%d %d", &vertex, &edge);
    insertInAdjList(adjList[vertex], edge);
  }

  return adjList;
}
