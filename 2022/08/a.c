#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct grid {
  int dim; // dimension (assume square grid)
  int* values;
} grid_t;

grid_t* init_grid(int dim) {
  grid_t* grid = calloc(1, sizeof(grid_t));
  grid->dim = dim;
  grid->values = calloc(dim, sizeof(int));
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
  return (x == 0 || x == (grid->dim - 1) || y == 0 || y == (grid->dim - 1));
}

int visible_in_row(grid_t* grid, int x, int y) {
  int height = grid->values[c2ind(grid, x, y)];
  int left, right;

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

  return left || right;
}

int visible_in_column(grid_t* grid, int x, int y) {
  int height = grid->values[c2ind(grid, x, y)];
  int up, down;
  
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
  
  return up || down;
}

int count_visible(grid_t* grid) {
  int visible = 0;
  
  for(int y = 0; y < grid->dim; y++) {
    for(int x = 0; x < grid->dim; x++) {
      if(is_edge(grid, x, y) || 
      visible_in_row(grid, x, y) || 
      visible_in_column(grid, x, y)) {
        visible++;
      }
    }
  }
  
  return visible;
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
  int visible = count_visible(grid);
  free_grid(grid);
  printf("Number of visible trees: %d\n", visible); // 1688
  return 0;
}
