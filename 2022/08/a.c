#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"

int is_visible(grid_t* grid, int x, int y) {
  if(is_edge(grid, x, y)) return 1;

  int height = grid->values[c2ind(grid, x, y)];
  int up, down, left, right;

  for(int i = 0; i < y; i++) {
    if(grid->values[c2ind(grid, x, i)] >= height) {
      up = 0;
      break;
    }
    up = 1;
  }

  for(int i = (y + 1); i < grid->dim; i++) {
    if(grid->values[c2ind(grid, x, i)] >= height) {
      down = 0;
      break;
    }
    down = 1;
  }

  for(int i = 0; i < x; i++) {
    if(grid->values[c2ind(grid, i, y)] >= height) {
      left = 0;
      break;
    }
    left = 1;
  }

  for(int i = (x + 1); i < grid->dim; i++) {
    if(grid->values[c2ind(grid, i, y)] >= height) {
      right = 0;
      break;
    }
    right = 1;
  }

  return up || down || left || right;
}

int count_visible(grid_t* grid) {
  int visible = 0;

  for(int y = 0; y < grid->dim; y++) {
    for(int x = 0; x < grid->dim; x++) {
      if(is_visible(grid, x, y)) visible++;
    }
  }

  return visible;
}

int main(void) {
  grid_t* grid = grid_from_file("input.txt");
  int visible = count_visible(grid);
  free_grid(grid);

  printf("Number of visible trees: %d\n", visible); // 1688
  return 0;
}
