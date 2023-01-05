// Totris - a squiffy version of that well known block game...
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct shape {
  int8_t line[4];
  uint8_t height;
} shape_t;

void make_shapes(shape_t**);
uint8_t board_collision(shape_t*, int8_t*, uint32_t);
void move_shape(shape_t*, uint8_t);
uint8_t can_move_shape(shape_t*, uint8_t, int8_t*, uint32_t);
uint32_t merge_shape(shape_t*, int8_t*, uint32_t);
