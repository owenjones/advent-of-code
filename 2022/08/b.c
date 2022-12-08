#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct grid {
  int dim; // dimension (assume square grid)
  int* values;
  int* scores;
} grid_t;

grid_t* init_grid(int dim) {
  grid_t* grid = calloc(1, sizeof(grid_t));
  grid->dim = dim;
  grid->values = calloc(dim*dim, sizeof(int));
  grid->scores = calloc(dim*dim, sizeof(int));
  return grid;
}

void free_grid(grid_t* grid) {
  free(grid->values);
  free(grid);
}

int c2ind(grid_t* grid, int x, int y) {
  return (grid->dim * y) + x;
}

int get_dimension(FILE* fptr) {
  char* input = calloc(256, sizeof(char));
  size_t bufsize;
  getline(&input, &bufsize, fptr);
  rewind(fptr);
  int dim = strlen(input) - 1;
  free(input);
  return dim;
}

void fill_grid(grid_t* grid, FILE* fptr) {
  char* input = calloc(256, sizeof(char));
  size_t bufsize;
  int y = 0;
  while(getline(&input, &bufsize, fptr) != -1) {
    for(int x = 0; x < grid->dim; x++) {
      grid->values[c2ind(grid, x, y)] = (input[x] - 48);
    }
    y++;
  }
}

int is_edge(grid_t* grid, int x, int y) {
  return (x <= 0 || x >= (grid->dim - 1) || y <= 0 || y >= (grid->dim - 1));
}

int calculate_row(grid_t* grid, int x, int y) {
  int height = grid->values[c2ind(grid, x, y)];
  int left = 1, right = 1;

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
  
  return left * right;
}

int calculate_column(grid_t* grid, int x, int y) {
  int height = grid->values[c2ind(grid, x, y)];
  int up = 1, down = 1;
  
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
  
  return up * down;
}

int calculate_scenic_score(grid_t* grid, int x, int y) {
  return (calculate_row(grid, x, y) * calculate_column(grid, x, y));
}

void get_scenic_scores(grid_t* grid) {
  for(int y = 0; y < grid->dim; y++) {
    for(int x = 0; x < grid->dim; x++) {
      if(is_edge(grid, x, y)) {
        grid->scores[c2ind(grid, x, y)] = 0;
      } else {
        grid->scores[c2ind(grid, x, y)] = calculate_scenic_score(grid, x, y);
      }
    }
  }
}

void sort_scores(grid_t* grid) {
  int n = (grid->dim * grid->dim);
  for(size_t i = 0; i < n; i++) {
    for(size_t j = 0; j < (n - i); j++) {
      if(grid->scores[j+1] > grid->scores[j]) {
        int temp = grid->scores[j];
        grid->scores[j] = grid->scores[j+1];
        grid->scores[j+1] = temp;
      }
    }
  }
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int dim = get_dimension(fptr);
  grid_t* grid = init_grid(dim);
  fill_grid(grid, fptr);
  fclose(fptr);
  get_scenic_scores(grid);
  sort_scores(grid);
  int best = grid->scores[0];
  free_grid(grid);
  printf("Best scenic score: %d\n", best); // 410400
  return 0;
}
