#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pathfind.h"

int main(void) {
  map_t* map = load_map("input.txt");
  int risk = calculate_risk(map);
  printf("Total risk: %d\n", risk); // 447
  free_map(map);
  return 0;
}
