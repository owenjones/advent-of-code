#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DIM 1000

typedef struct coord {
  int x;
  int y;
} coord_t;

int c2ind(int x, int y) {
  return (y * DIM) + x;
}

void set_direction(char direction, int* x, int* y) {
  // set (x,y) vector depending on input direction
  switch(direction) {
    case 'U':
      *x = 0;
      *y = 1;
      break;

    case 'D':
      *x = 0;
      *y = -1;
      break;

    case 'L':
      *x = -1;
      *y = 0;
      break;

    case 'R':
      *x = 1;
      *y = 0;
      break;
  }
}

int is_neighbour(coord_t* a, coord_t* b) {
  int x = (abs(b->x - a->x) <= 1);
  int y = (abs(b->y - a->y) <= 1);
  return x && y;
}

int manhattan_distance(coord_t* a, coord_t* b) {
  int x = abs(b->x - a->x);
  int y = abs(b->y - a->y);
  return x + y;
}

void mark_visited(int* list, int x, int y) {
  list[c2ind(x, y)] = 1;
}

void set_diagonal_direction(coord_t* head, coord_t* tail, int* x, int* y) {
  // vector between head & tail - move 1 in that x&y
  *x = copysign(1, (head->x - tail->x));
  *y = copysign(1, (head->y - tail->y));
}

int count_visited(int* list) {
  int total = 0;
  for(size_t i = 0; i < (DIM * DIM); i++) total += list[i];
  return total;
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  // grid, origin in bottom left
  int* squares = calloc((DIM * DIM), sizeof(int));

  // head of rope, starts at the origin
  coord_t* head = calloc(1, sizeof(coord_t));
  head->x = 500;
  head->y = 500;
  
  // tail of rope, also starts at the origin
  coord_t* tail = calloc(1, sizeof(coord_t));
  tail->x = 500;
  tail->y = 500;
  mark_visited(squares, 500, 500);

  char direction;
  int steps, distance, x, y;
  size_t i;
  
  while(fscanf(fptr, "%c %d\n", &direction, &steps) > 0) {

    for(i = 0; i < steps; i++) {
      // move head 1 step in direction
      set_direction(direction, &x, &y);
      head->x += x;
      head->y += y;
      
      // don't need to move tail unless it's not adjacant/underneath the head
      if(is_neighbour(head, tail)) {
        continue;
      }
      
      if(manhattan_distance(head, tail) > 2) {
        // head is diagonal from tail, need to move tail a step diagonally
        set_diagonal_direction(head, tail, &x, &y);
      }
        
      tail->x += x;
      tail->y += y;
      
      mark_visited(squares, tail->x, tail->y);
    }
  }

  fclose(fptr);
  free(head);
  free(tail);
  free(squares);

  printf("Squares visited: %d\n", count_visited(squares)); // 6522
  
  return 0;
}
