#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pathfind.h"

int main(void) {
  map_t* map = load_map("input.txt");
  int steps = steps_to_end(map);
  free_map(map);
  printf("Number of steps in shortest route from start to end = %d\n", steps);
  return 0;
}
