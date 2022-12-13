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
      
      for(size_t i = 0; i < 5; i++) {
        int n = (h + i) % 9;
        n = (n == 0) ? 9 : n;
        
        map->heights[(y * map->w) + (i * old->w) + x] = n;
        //map->heights[() + x] = n;
      }
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
    //exit(1);
  }
  
  // int risk = calculate_risk(map);
  // printf("Total risk: %d\n", risk);
  free_map(map);
  return 0;
}
