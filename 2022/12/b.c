#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pathfind.h"

int main(void) {
  map_t* map = load_map("input.txt");
  int steps = find_hiking_trail(map);
  free_map(map);
  printf("Number of steps in shortest hiking trail = %d\n", steps); // 439
  return 0;
}
