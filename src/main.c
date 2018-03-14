#include <stdio.h>
#include "list.h"

int main() {
  Node *ola = buildAdjList();
  Node *copy = transposeAdjList(ola);
  showAdjList(copy);
  showAdjList(ola);
  freeAdjList(copy);
  freeAdjList(ola);
  return 0;
}
