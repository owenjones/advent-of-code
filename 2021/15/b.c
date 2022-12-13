#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pathfind.h"

map_t* expand_map(map_t* old) {
  map_t* map = new_map();
  map->w = (old->w * 5);
  map->h = (old->h * 5);
  
  for(size_t y = 0; y < old->h; y++) {
    for(size_t x = 0; x < old->w; x++) {
      int h = old->heights[(y * old->w) + x];
      
      
    }
  }
  
  return map;
}

int main(void) {
  map_t* small = load_map("test_input.txt");
  map_t* map = expand_map(small);
  free_map(small);
  
  for(size_t y = 0; y < map->h; y++) {
    for(size_t x = 0; x < map->w; x++) {
      printf("%d", map->heights[(y * map->w) + x]);
    }
    printf("\n");
  }
  
  // int risk = calculate_risk(map);
  // printf("Total risk: %d\n", risk);
  free_map(map);
  return 0;
}
