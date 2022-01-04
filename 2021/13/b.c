#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct grid {
  int x;
  int y;
  int* points;
} grid_t;

int c2ind(grid_t* grid, int x, int y) {
  int ind = x + (grid->x * y);
  int bounds = (grid->x + 1) * (grid->y + 1);
  if(ind > bounds) {
    printf("Index (%i) out of bounds (max %i)\n", ind, bounds);
    exit(1);
  }
  return ind;
}

grid_t* fold(grid_t* grid, char axis, int line) {
  grid_t* new = calloc(1, sizeof(grid_t));
  new->x = (axis == 'x') ? line : grid->x;
  new->y = (axis == 'y') ? line : grid->y;
  new->points = calloc((new->x + 1) * (new->y + 1), sizeof(int));

  int x, y;
  for(size_t j = 0; j < grid->y; j++) {
    for(size_t i = 0; i < grid->x; i++) {
      x = (axis == 'x') ? ((i > new->x) ? (new->x - (i - new->x)) : i) : i;
      y = (axis == 'y') ? ((j > new->y) ? (new->y - (j - new->y)) : j) : j;
      new->points[c2ind(new, x, y)] |= grid->points[c2ind(grid, i, j)];
    }
  }

  free(grid->points);
  free(grid);
  return new;
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  grid_t *grid = calloc(1, sizeof(grid_t));
  grid->x = 1311;
  grid->y = 895;
  grid->points = calloc(grid->x * grid->y, sizeof(int));

  int x, y;
  while(fscanf(fptr, "%i,%i\n", &x, &y) > 0) {
    grid->points[c2ind(grid, x, y)] = 1;
  }

  char axis;
  int line;
  while(fscanf(fptr, "fold along %c=%i\n", &axis, &line) > 0) grid = fold(grid, axis, line);

  fclose(fptr);

  // HZLEHJRK
  for(size_t i = 0; i < (grid->x * grid->y); i++) {
    printf("%s", (grid->points[i] == 1) ? " * " : "   ");
    if((i + 1) % grid->x == 0) printf("\n");
  }

  free(grid->points);
  free(grid);

  return 0;
}
