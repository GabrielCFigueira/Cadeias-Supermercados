#include <stdio.h>
#include "adj.h"

int main() {
  Graph ola = buildGraph();
  Graph copy = transposeGraph(ola);
  showGraph(ola);
  showGraph(copy);
  freeGraph(ola);
  freeGraph(copy);
  return 0;
}
