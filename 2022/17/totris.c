#include "totris.h"

void make_shapes(shape_t** shapes) {
  for(size_t i = 0; i < 5; i++) shapes[i] = calloc(1, sizeof(shape_t));

  // -
  shapes[0]->line[0] = 0;
  shapes[0]->line[1] = 0;
  shapes[0]->line[2] = 0;
  shapes[0]->line[3] = 30;
  shapes[0]->height  = 1;

  // +
  shapes[1]->line[0] = 0;
  shapes[1]->line[1] = 8;
  shapes[1]->line[2] = 28;
  shapes[1]->line[3] = 8;
  shapes[1]->height  = 3;

  // backwards L
  shapes[2]->line[0] = 0;
  shapes[2]->line[1] = 4;
  shapes[2]->line[2] = 4;
  shapes[2]->line[3] = 28;
  shapes[2]->height  = 3;

  // |
  shapes[3]->line[0] = 16;
  shapes[3]->line[1] = 16;
  shapes[3]->line[2] = 16;
  shapes[3]->line[3] = 16;
  shapes[3]->height  = 4;

  // ▮
  shapes[4]->line[0] = 0;
  shapes[4]->line[1] = 0;
  shapes[4]->line[2] = 24;
  shapes[4]->line[3] = 24;
  shapes[4]->height  = 2;
}

uint8_t board_collision(shape_t* shape, int8_t* board, uint32_t y) {
  // take four lines of shape in reverse order and bitwise-and these with four lines of
  // board starting from y (y, y+1, y+2...), bitwise-or the four results together
  // to detect if there's a collision
  return (
    (shape->line[3] & board[y]) |
    (shape->line[2] & board[(y + 1)]) |
    (shape->line[1] & board[(y + 2)]) |
    (shape->line[0] & board[(y + 3)])
  ) > 0;
}

void move_shape(shape_t* shape, uint8_t direction) {
  for(size_t i = 0; i < 4; i++) {
    if(direction == 0) shape->line[i] <<= 1;
    else shape->line[i] >>= 1;
  }
}

uint8_t can_move_shape(shape_t* shape, uint8_t direction, int8_t* board, uint32_t y) {
  // test walls (leftmost/rightmost bit is 1)
  int8_t lines = shape->line[0] | shape->line[1] | shape->line[2] | shape->line[3];
  if(direction == 0) { // <
    if((lines >> 6) == 1) return 0;
  } else if(direction == 1) { // >
    if((lines & 00000001) == 1) return 0;
  }

  // test against other shapes (clone the shape, make the move, check for collision)
  shape_t* clone = calloc(1, sizeof(shape_t));
  memcpy(clone, shape, sizeof(shape_t));
  move_shape(clone, direction);
  uint8_t collision = board_collision(clone, board, y);
  free(clone);

  return !collision;
}

uint32_t merge_shape(shape_t* shape, int8_t* board, uint32_t y) {
  board[y]       |= shape->line[3];
  board[(y + 1)] |= shape->line[2];
  board[(y + 2)] |= shape->line[1];
  board[(y + 3)] |= shape->line[0];
  return y + shape->height;
}
