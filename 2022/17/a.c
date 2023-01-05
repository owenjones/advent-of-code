#include "totris.h"

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
  uint32_t s = 0, height = 0, i = 0, y, at_rest;

  for(size_t r = 0; r < 2022; r++) {
    memcpy(shape, shapes[s], sizeof(shape_t));
    s = (s + 1) % 5;

    at_rest = 0;
    y = height + 3; // start bottom of shape 3 lines above current highest point
    while(!at_rest) {
      // check if can move the shape (and if so, do it)
      if(can_move_shape(shape, instructions[i], board, y)) move_shape(shape, instructions[i]);
      i = (i + 1) % n_instructions;

      // then we drop the shape
      if(y == 0) {
        // edge case: if y == 0 (bottom of the cavern) then just drop shape right here
        at_rest = merge_shape(shape, board, 0);
      } else if(board_collision(shape, board, (y - 1))) {
        // if dropping shape would cause a collision merge it into the board
        at_rest = merge_shape(shape, board, y);
      } else {
        // otherwise, drop the shape a line and continue
        y--;
      }

      if(at_rest) {
        // need to test if shape drop has actually increased the height
        // (and isn't a sneaky | going down the side)
        height = (at_rest > height) ? at_rest : height;
        break;
      }
    }
  }

  free(board);
  free(shape);
  for(size_t i = 0; i < 5; i++) free(shapes[i]);

  printf("Total height after 2022 rounds = %i\n", height); // 3191
  return 0;
}
