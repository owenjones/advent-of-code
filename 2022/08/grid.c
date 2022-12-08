#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"

grid_t* grid_from_file(char* file) {
  FILE* fptr;
  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  char* input = NULL;
  size_t bufsize = 0;

  // read first line to work out dimensions of grid and initialise
  getline(&input, &bufsize, fptr);
  int dim = strlen(input) - 1;
  grid_t* grid = calloc(1, sizeof(grid_t));
  grid->dim = dim;
  grid->values = calloc((dim * dim), sizeof(int));

  // rewind file pointer to start and then read each line
  rewind(fptr);
  int y = 0;
  while(getline(&input, &bufsize, fptr) != -1) {
    for(int x = 0; x < grid->dim; x++) {
      grid->values[c2ind(grid, x, y)] = (input[x] - 48);
    }
    y++;
  }

  fclose(fptr);
  free(input);

  return grid;
}

void free_grid(grid_t* grid) {
  free(grid->values);
  free(grid);
}

int c2ind(grid_t* grid, int x, int y) {
  return (grid->dim * y) + x;
}

int is_edge(grid_t* grid, int x, int y) {
  return (x == 0 || x == (grid->dim - 1) || y == 0 || y == (grid->dim - 1));
}

int is_out_of_bounds(grid_t* grid, int x, int y) {
  return (x < 0 || x > (grid->dim - 1) || y < 0 || y > (grid->dim - 1));
}
