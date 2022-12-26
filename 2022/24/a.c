#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// #define INPUT "test_input.txt"
// #define GRIDX 5
// #define GRIDY 5

// #define INPUT "test_input_2.txt"
// #define GRIDX 6
// #define GRIDY 4

#define INPUT "input.txt"
#define GRIDX 120
#define GRIDY 25

typedef struct blizzard {
  uint8_t x, y, direction;
  // Direction: 0 = N, 1 = E, 2 = S, 3 = W
} blizzard_t;

typedef struct blizzards {
  uint16_t count;
  blizzard_t** blizzard;
} blizzards_t;

typedef struct grid {
  uint16_t time;
  uint8_t cell[GRIDX][GRIDY];
} grid_t;

typedef struct node {
  int8_t x, y;
  uint16_t time;
} node_t;

typedef struct list {
  uint32_t n;
  node_t** nodes;
} list_t;

grid_t* blank_grid() {
  grid_t* grid = calloc(1, sizeof(grid_t));
  for(size_t j = 0; j < GRIDY; j++) {
    for(size_t i = 0; i < GRIDX; i++) {
      grid->cell[i][j] = 0;
    }
  }
  return grid;
}

grid_t* grid_at_time(grid_t** grids, blizzards_t* blizzards, uint16_t time) {
  if(grids[time] != NULL) return grids[time];
  
  grid_t* grid = blank_grid();
  grid->time = time;
  
  uint8_t x, y;
  blizzard_t* blizzard;
  for(size_t i = 0; i < blizzards->count; i++) {
    blizzard = blizzards->blizzard[i];
    // work out actual (x, y) from initial position and number of minutes of movement
    
    switch(blizzard->direction) {
      case 0:
        x = blizzard->x;
        y = (GRIDY + (blizzard->y - time) % GRIDY) % GRIDY;
        break;
        
      case 1:
        x = (blizzard->x + time) % GRIDX;
        y = blizzard->y;
        break;
        
      case 2:
        x = blizzard->x;
        y = (blizzard->y + time) % GRIDY;
        break;
        
      case 3:
        x = (GRIDX + (blizzard->x - time) % GRIDX) % GRIDX;
        y = blizzard->y;
        break;
    }
    
    grid->cell[x][y] += 1;
  }
  
  grids[time] = grid;
  return grid;
}

void print_grid(grid_t* grid) {
  for(size_t j = 0; j < GRIDY; j++) {
    for(size_t i = 0; i < GRIDX; i++) {
      printf("%d", grid->cell[i][j]);
    }
    printf("\n");
  }
}

node_t* new_node(int8_t x, int8_t y, uint16_t time) {
  node_t* node = calloc(1, sizeof(node_t));
  node->x = x;
  node->y = y;
  node->time = time;
  return node;
}

list_t* new_list() {
  // return an empty node list
  list_t* list = calloc(1, sizeof(list_t));
  list->nodes = calloc(10000, sizeof(node_t*));
  return list;
}

void free_list(list_t* list) {
  for(size_t n = 0; n < list->n; n++) free(list->nodes[n]);
  free(list->nodes);
  free(list);
}

node_t* remove_node_at(list_t* list, int i) {
  // remove node at position i from the list and return it
  node_t* h = list->nodes[i];
  for(size_t n = i; n < (list->n - 1); n++) {
    list->nodes[n] = list->nodes[(n + 1)];
  }
  list->n--;
  return h;
}

node_t* get_head_node(list_t* list) {
  // take first element off the list
  return remove_node_at(list, 0);
}

void append_node(list_t* list, node_t* node) {
  // append new element to end of the list
  list->nodes[list->n++] = node;
}

int already_in_list(list_t* list, int8_t x, int8_t y, uint16_t time) {
  for(size_t n = 0; n < list->n; n++) {
    if(list->nodes[n]->x == x && list->nodes[n]->y == y && list->nodes[n]->time == time) {
      return 1;
    }
  }
  return 0;
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen(INPUT, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  blizzards_t* blizzards = calloc(1, sizeof(blizzards_t));
  blizzards->count = 0;
  blizzards->blizzard = calloc(4000, sizeof(blizzard_t*));

  char* input = NULL;
  size_t bufsize = 0, i, j = 0;
  blizzard_t* blizzard;
  while(getline(&input, &bufsize, fptr) != -1) {
    if(input[2] == '#') continue; // skip first & last boundary line
    for(i = 1; i < (strlen(input) - 2); i++) {
      if(input[i] != '.') {
        blizzard = calloc(1, sizeof(blizzard));
        blizzard->x = (i - 1);
        blizzard->y = j;
        switch(input[i]) {
          case '^':
            blizzard->direction = 0;
            break;
            
          case '>':
            blizzard->direction = 1;
            break;
            
          case 'v':
            blizzard->direction = 2;
            break;
            
          case '<':
            blizzard->direction = 3;
            break;
        }
        
      blizzards->blizzard[blizzards->count++] = blizzard;
      }
    }
    j++;
  }
  free(input);
  fclose(fptr);

  grid_t* grids[10000];
  list_t* search = new_list();
  
  node_t* start = new_node(0, -1, 0);
  append_node(search, start);
  
  uint32_t end = 0;
  node_t* current;
  grid_t* grid;
  int8_t x, y;
  while(search->n > 0 && !end) {
    current = get_head_node(search);
    grid = grid_at_time(grids, blizzards, (current->time + 1));
    
    for(i = 0; i < 5; i++) {
      switch(i) {
        case 0:
          x = current->x;
          y = (current->y - 1);
          break;
        case 1:
          x = (current->x + 1);
          y = current->y;
          break;
        case 2:
          x = current->x;
          y = (current->y + 1);
          break;
        case 3:
          x = (current->x - 1);
          y = current->y;
          break;
        case 4:
          x = current->x;
          y = current->y;
          break;
      }

      if(x == (GRIDX - 1) && y == GRIDY) {
        end = (current->time + 1);
        break;
      } else if((x == 0 && y == -1) || (x >= 0 && x < GRIDX && y >= 0 && y < GRIDY)) {
        if((x == 0 && y == -1) || grid->cell[x][y] == 0) {
          if(!already_in_list(search, x, y, (current->time + 1))) {
            append_node(search, new_node(x, y, (current->time + 1)));
          }
        }
      }
    }
    free(current);
  }
  
  free_list(search);
  for(size_t x = 0; x <= (end + 1); x++) free(grids[x]);
  for(size_t x = 0; x < blizzards->count; x++) free(blizzards->blizzard[x]);
  free(blizzards->blizzard);
  free(blizzards);

  printf("Minutes to reach the end = %d\n", end); // 299
  return 0;
}
