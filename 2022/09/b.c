#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DIM 1000

typedef struct coord {
  int id;
  int x;
  int y;
  struct coord* child;
} coord_t;

int c2ind(int x, int y) {
  return (y * DIM) + x;
}

void set_initial_direction(char direction, int* x, int* y) {
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

void set_direction(coord_t* head, coord_t* tail, int* x, int* y) {
  // vector between head & tail - move 1 in that x&y
  *x = floor((tail->x - head->x) / 2);
  *y = floor((tail->y - head->y) / 2);
}

int is_neighbour(coord_t* a, coord_t* b) {
  int x = (abs(b->x - a->x) <= 1);
  int y = (abs(b->y - a->y) <= 1);
  return x && y;
}

void mark_visited(int* list, int x, int y) {
  list[c2ind(x, y)] = 1;
}

int count_visited(int* list) {
  int total = 0;
  for(size_t i = 0; i < (DIM * DIM); i++) total += list[i];
  return total;
}

void free_knots(coord_t* start) {
  coord_t *ptr = start, *temp;
  while(ptr != NULL) {
    temp = ptr->child;
    free(ptr);
    ptr = temp;
  }
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  // grid, origin right in the middle (500, 500)
  int* squares = calloc((DIM * DIM), sizeof(int));

  // head of rope, starts at the origin
  coord_t* head = calloc(1, sizeof(coord_t));
  head->id = 'H';
  head->x = 500;
  head->y = 500;
  head->child = NULL;
  
  // add 9 additional knots beneath head (all on the origin)
  coord_t* parent = head;
  coord_t* child;
  for(size_t i = 0; i < 9; i++) {
    child = calloc(1, sizeof(coord_t));
    child->id = (i + 1);
    child->x = 500;
    child->y = 500;
    child->child = NULL;
    
    parent->child = child;
    parent = child;
  }
  
  // pre-mark origin as we don't move in to it
  mark_visited(squares, 500, 500);

  char direction;
  int steps, x, y;
  size_t i;
  coord_t *current, *next;
  
  while(fscanf(fptr, "%c %d\n", &direction, &steps) > 0) {
    for(i = 0; i < steps; i++) {
      // move head 1 step in direction
      set_initial_direction(direction, &x, &y);
      head->x += x;
      head->y += y;
      
      current = head;
      next = head->child;
      while(next != NULL) {
        if(is_neighbour(current, next)) break;
        
        set_direction(current, next, &x, &y);
        next->x = current->x + x;
        next->y = current->y + y;

        if(next->child == NULL) {
          mark_visited(squares, next->x, next->y);
        }

        current = next;
        next = next->child;
      }
    }
  }
  fclose(fptr);
  free_knots(head);
  free(squares);

  printf("Squares visited: %d\n", count_visited(squares)); // 2717
  
  return 0;
}
