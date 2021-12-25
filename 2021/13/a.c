#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct grid {
  int x;
  int y;
  int* points;
} grid_t;

int c2ind(grid_t* grid, int x, int y) {
  return x + (grid->x * y);
}

void fold(grid_t* grid, char axis, int line) {
  grid_t* new = calloc(1, sizeof(grid_t));
  new->x = (axis == 'x') ? line : grid->x;
  new->y = (axis == 'y') ? line : grid->y;
  new->points = (int*) calloc(new->x * new->y, sizeof(int));
  
  int x, y;
  for(size_t j = 0; j < grid->y; j++) {
    for(size_t i = 0; i < grid->x; i++) {
      x = (axis == 'x') ? ((i > new->x) ? (new->x - (i - new->x)) : i) : i;
      y = (axis == 'y') ? ((j > new->y) ? (new->y - (j - new->y)) : j) : j;      
      new->points[c2ind(new, x, y)] |= grid->points[c2ind(grid, i, j)];
    }
  }
  
  *grid = *new;
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  grid_t *grid = (grid_t*) calloc(1, sizeof(grid_t));
  grid->x = 1310;
  grid->y = 894;
  grid->points = (int*) calloc(grid->x * grid->y, sizeof(int));

  int x, y;
  while(fscanf(fptr, "%i,%i\n", &x, &y) > 0) {
    grid->points[c2ind(grid, x, y)] = 1;
  }

  char axis;
  int line;
  fscanf(fptr, "fold along %c=%i\n", &axis, &line);
  fold(grid, axis, line);
  
  fclose(fptr);

  int dots = 0;  
  for(size_t i = 0; i < (grid->x * grid->y); i++) dots += grid->points[i];
  printf("Number of dots visible: %i\n", dots); // 753
  
  free(grid);
  
  return 0;
}
