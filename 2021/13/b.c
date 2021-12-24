#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum axis {x, y};

typedef struct grid {
  int x;
  int y;
  int* points;
} grid_t;

typedef struct fold {
  enum axis axis;
  int line;
} fold_t;

int c2ind(grid_t* grid, int x, int y) {
  return x + (grid->x * y);
}

// Assumptions:
// * each fold will be exactly in half (seems to fit with values at first glance)

grid_t* fold_x(grid_t* grid, fold_t* fold) {
  grid_t* new = calloc(1, sizeof(fold_t));
  new->x = fold->line;
  new->y = grid->y;
  new->points = (int*) calloc(new->x * new->y, sizeof(int));
  
  int x, y;
  for(size_t j = 0; j < grid->y; j++) {
    for(size_t i = 0; i < grid->x; i++) {
      x = (i > new->x) ? (new->x - (i - new->x)) : i; // reflect in x
      y = j;
      
      new->points[c2ind(new, x, y)] |= grid->points[c2ind(grid, i, j)];
    }
  }
  
  free(grid);
  return new;
}

grid_t* fold_y(grid_t* grid, fold_t* fold) {
  grid_t* new = calloc(1, sizeof(fold_t));
  new->x = grid->x;
  new->y = fold->line;
  new->points = (int*) calloc(new->x * new->y, sizeof(int));
  
  int x, y;
  for(size_t j = 0; j < grid->y; j++) {
    for(size_t i = 0; i < grid->x; i++) {
      x = i;
      y = (j > new->y) ? (new->y - (j - new->y)) : j; // reflect in y
      
      new->points[c2ind(new, x, y)] |= grid->points[c2ind(grid, i, j)];
    }
  }
  
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

  grid_t *grid = (grid_t*) calloc(1, sizeof(grid_t));
  grid->x = 1310;
  grid->y = 894;
  grid->points = (int*) calloc(grid->x * grid->y, sizeof(int));

  int x, y;
  int i = 0;
  while(fscanf(fptr, "%i,%i\n", &x, &y) > 0) {
    grid->points[c2ind(grid, x, y)] = 1;
  }

  fold_t **folds = (fold_t**) calloc(12, sizeof(fold_t*));
  char axis;
  int line, nfolds = 0;
  while(fscanf(fptr, "fold along %c=%i\n", &axis, &line) > 0) {
    fold_t* fold = (fold_t*) calloc(1, sizeof(fold_t));
    fold->line = line;
    switch(axis) {
      case 'x': fold->axis = x; break;
      case 'y': fold->axis = y; break;
    }
    
    folds[nfolds++] = fold;
  }
  
  fclose(fptr);

  for(size_t f = 0; f < nfolds; f++) { // just look at first fold for now
    if(folds[f]->axis == x) {
      grid = fold_x(grid, folds[f]);
    } else if(folds[f]->axis == y) {
      grid = fold_y(grid, folds[f]);
    }
  }

  // HZLEHJRK
  for(size_t i = 0; i < (grid->x * grid->y); i++) {
    printf("%s", (grid->points[i] == 1) ? " * " : "   ");
    if((i + 1) % grid->x == 0) printf("\n");
  }
  
  return 0;
}
