#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct shape {
  int8_t line[4];
} shape_t;

void make_shapes(shape_t** shapes) {
  for(size_t i = 0; i < 5; i++) {
    shapes[i] = calloc(1, sizeof(shape_t));
    
    switch(i) {
      case 0: // -
        shapes[i]->line[0] = 0;
        shapes[i]->line[1] = 0;
        shapes[i]->line[2] = 0;
        shapes[i]->line[3] = 30;
        break;
        
      case 1: // +
        shapes[i]->line[0] = 0;
        shapes[i]->line[1] = 8;
        shapes[i]->line[2] = 28;
        shapes[i]->line[3] = 8;
        break;
        
      case 2: // backwards L
        shapes[i]->line[0] = 0;
        shapes[i]->line[1] = 4;
        shapes[i]->line[2] = 4;
        shapes[i]->line[3] = 28;
        break;
        
      case 3: // |
        shapes[i]->line[0] = 16;
        shapes[i]->line[1] = 16;
        shapes[i]->line[2] = 16;
        shapes[i]->line[3] = 16;
        break;
        
      case 4: // ▮
        shapes[i]->line[0] = 0;
        shapes[i]->line[1] = 0;
        shapes[i]->line[2] = 24;
        shapes[i]->line[3] = 24;
        break;
    }
  }
}

uint8_t board_collision(shape_t* shape, int8_t* board, uint32_t y) {
  // take four lines of shape in reverse order and bitwise-and these with four lines of
  // board starting from y (y, y+1, y+1...), bitwise-or the four results together
  // to detect if there's a collision
  uint8_t collision = (
    (shape->line[3] & board[y]) |
    (shape->line[2] & board[(y + 1)]) |
    (shape->line[1] & board[(y + 2)]) |
    (shape->line[0] & board[(y + 3)])
  );
  
  return (collision > 0);
}

void move_shape(shape_t* shape, uint8_t direction) {
  // assumes we've already checked that the move is valid
  for(size_t i = 0; i < 4; i++) {
    if(direction == 0) shape->line[i] <<= 1;
    else shape->line[i] >>= 1;
  }
}

uint8_t can_move_shape(shape_t* shape, uint8_t direction, int8_t* board, uint32_t y) {
  // test walls (left most/right most bit is a 1)
  int8_t lines = shape->line[0] | shape->line[1] | shape->line[2] | shape->line[3];
  if(direction == 0) { // <
    if((lines >> 6) == 1) return 0;
  } else if(direction == 1) { // >
    if((lines & 00000001) == 1) return 0;
  }
  
  // test other shapes (clone, make the move, check for collision)
  shape_t* moved = calloc(1, sizeof(shape_t));
  memcpy(moved, shape, sizeof(shape_t));
  move_shape(moved, direction);
  uint8_t collision = board_collision(moved, board, y);
  free(moved);
  
  return !collision;
}

uint32_t merge_shape(shape_t* shape, int8_t* board, uint32_t y) {
  board[y]       |= shape->line[3];
  board[(y + 1)] |= shape->line[2];
  board[(y + 2)] |= shape->line[1];
  board[(y + 3)] |= shape->line[0];
  
  // work out where the top of the shape will be
  uint32_t top = y + 4;
  if(shape->line[0] == 0) top--;
  if(shape->line[1] == 0) top--;
  if(shape->line[2] == 0) top--;
  return top;
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  char* input = NULL;
  size_t bufsize = 0;
  if(getline(&input, &bufsize, fptr) == -1) {
    printf("Error reading input line\n");
    exit(1);
  }
  fclose(fptr);

  uint8_t instructions[15000];
  size_t n_instructions = 0;
  for(size_t i = 0; i < strlen(input); i++) {
    char c = input[i];
    if(c == 10) continue; // skip new lines
    instructions[i] = (c == 60) ? 0 : 1; // < = 0, > = 1
    n_instructions++;
  }
  free(input);
  
  shape_t* shapes[5];
  make_shapes(shapes);
  shape_t* shape = malloc(sizeof(shape_t));
  int8_t* board = calloc(10000, sizeof(int8_t)); // ceiling for 2022 shape drops is 8088 (4*2022)
  uint32_t s = 0, height = 0, i = 0, y, dropped;
  
  for(size_t r = 0; r < 2022; r++) {
    memcpy(shape, shapes[s], sizeof(shape_t));
    
    y = height + 3; // start bottom of shape 3 lines above current highest point
    while(1) {
      if(can_move_shape(shape, instructions[i], board, y)) move_shape(shape, instructions[i]);
      i = (i + 1) % n_instructions;
      
      // then we test if shape can drop
      if(y == 0) {
        // edge case: if y == 0 then drop shape right here
        dropped = merge_shape(shape, board, 0);
        height = (dropped > height) ? dropped : height;
        break;
      } else if(board_collision(shape, board, (y - 1))) {
        // if it can't, merge shape into board and exit round
        dropped = merge_shape(shape, board, y);
        height = (dropped > height) ? dropped : height;
        break;
      } else {
        // if it can - drop it and continue
        y--;
      }
    }
    
    s = (s + 1) % 5;
  }
  
  free(board);
  free(shape);
  for(size_t i = 0; i < 5; i++) free(shapes[i]);
  
  printf("Total height after 2022 rounds = %i\n", height); // 3191
  return 0;
}
