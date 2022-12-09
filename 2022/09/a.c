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

int is_adjacent(coord_t* a, coord_t* b) {
  int x = (abs(b->x - a->x) == 1);
  int y = (abs(b->y - a->y) == 1);
  return x && y;
}

int is_on_top(coord_t* a, coord_t* b) {
  return (a->x == b->x) && (a->y == b->y);
}

int on_same_plane(char direction, char previous) {
  if((direction == 'L' && previous == 'R') || 
  (direction == 'R' && previous == 'L')) return 1;
  
  if((direction == 'U' && previous == 'D') || 
  (direction == 'D' && previous == 'U')) return 1;
  
  return 0;
}

int manhattan_distance(coord_t* a, coord_t* b) {
  int x = abs(b->x - a->x);
  int y = abs(b->y - a->y);
  return x + y;
}

int is_diagonal(coord_t* a, coord_t* b) {
  return is_adjacent(a, b) && (manhattan_distance(a, b) == 2);
}

void mark_visited(int* list, int x, int y) {
  printf("Visited (%d, %d)\n", x, y);
  list[c2ind(x, y)] = 1;
}

void move_head(coord_t* head, char direction, int steps) {
  int x, y;
  set_direction(direction, &x, &y);
  head->x += (x * steps);
  head->y += (y * steps);
}

void move_tail_linearly(coord_t* point, char direction, int steps, int* squares) {
  if(steps < 0) return; // don't think this should happen, but while testing...
  
  int x, y;
  set_direction(direction, &x, &y);
  for(size_t i = 0; i < steps; i++) {
    point->x += x;
    point->y += y;
    mark_visited(squares, point->x, point->y);
  }
}

void move_tail_diagonally(coord_t* head, coord_t* tail, int* squares) {
  // vector between head & tail - move 1 in that x&y
  int dx = copysign(1, (head->x - tail->x));
  int dy = copysign(1, (head->y - tail->y));
  
  tail->x += dx;
  tail->y += dy;
  mark_visited(squares, tail->x, tail->y);
}

int count_visited(int* list) {
  int total = 0;
  for(size_t i = 0; i < (DIM * DIM); i++) total += list[i];
  return total;
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("test_input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  // grid, origin in bottom left
  int* squares = calloc((DIM * DIM), sizeof(int));

  // head of rope, starts at the origin
  coord_t* head = calloc(1, sizeof(coord_t));
  head->x = 0;
  head->y = 0;
  
  // tail of rope, also starts at the origin
  coord_t* tail = calloc(1, sizeof(coord_t));
  tail->x = -1;
  tail->y = 0;

  char direction;
  char previous = 'R';
  int steps, distance;
  while(fscanf(fptr, "%c %d\n", &direction, &steps) > 0) {
    printf("Read line (%c %d) - ", direction, steps);

    move_head(head, direction, steps);
    
    // if head is now adjacent to tail (or on top of it) then we don't need to move it
    if(is_adjacent(head, tail) || is_on_top(head, tail)) {
      printf("head is adjacent/on top\n");
      continue;
    }
  
    if(direction == previous) {
      printf("moving in same direction\n");
      move_tail_linearly(tail, direction, steps, squares);
    } else if(on_same_plane(direction, previous)) {
      printf("moving in same plane/opposite direction\n");
      distance = manhattan_distance(head, tail);
      move_tail_linearly(tail, direction, (steps - 3), squares);
    } else {
      printf("moving in opposite plane\n");
      move_tail_diagonally(head, tail, squares);
      move_tail_linearly(tail, direction, (steps - 2), squares);
    }

    previous = direction;
  }

  fclose(fptr);

  printf("Squares visited: %d\n", count_visited(squares));
  
  // 6411 - too low
  // 6415 - X
  // 6416 - X
  // 7203 - too high
  
  return 0;
}
