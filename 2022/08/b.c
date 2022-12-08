#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"

int calculate_scenic_score(grid_t* grid, int x, int y) {
  int height = grid->values[c2ind(grid, x, y)];
  int up = 1, down = 1, left = 1, right = 1;

  for(size_t i = (y - 1); i >= 0; i--) {
    if(is_edge(grid, x, i)) break;
    if(grid->values[c2ind(grid, x, i)] >= height) break;
    else up++;
  }

  for(size_t i = (y + 1); i < grid->dim; i++) {
    if(is_edge(grid, x, i)) break;
    if(grid->values[c2ind(grid, x, i)] >= height) break;
    else down++;
  }

  for(size_t i = (x - 1); i >= 0; i--) {
    if(is_edge(grid, i, y)) break;
    if(grid->values[c2ind(grid, i, y)] >= height) break;
    else left++;
  }

  for(size_t i = (x + 1); i < grid->dim; i++) {
    if(is_edge(grid, i, y)) break;
    if(grid->values[c2ind(grid, i, y)] >= height) break;
    else right++;
  }

  return up * down * left * right;
}

void get_scenic_scores(grid_t* grid, int* scores) {
  for(int y = 0; y < grid->dim; y++) {
    for(int x = 0; x < grid->dim; x++) {
      if(is_edge(grid, x, y)) {
        scores[c2ind(grid, x, y)] = 0;
      } else {
        scores[c2ind(grid, x, y)] = calculate_scenic_score(grid, x, y);
      }
    }
  }
}

void sort_scores(int* scores, int dim) {
  int n = (dim * dim);
  for(size_t i = 0; i < n - 1; i++) {
    for(size_t j = 0; j < (n - 1 - i); j++) {
      if(scores[j+1] > scores[j]) {
        int temp = scores[j];
        scores[j] = scores[j+1];
        scores[j+1] = temp;
      }
    }
  }
}

int main(void) {
  grid_t* grid = grid_from_file("input.txt");
  int* scores = calloc((grid->dim * grid->dim), sizeof(int));

  get_scenic_scores(grid, scores);
  sort_scores(scores, grid->dim);
  int best = scores[0];
  free(scores);
  free_grid(grid);

  printf("Best scenic score: %d\n", best); // 410400
  return 0;
}
