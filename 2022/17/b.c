#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// how many lines of the board do we save in state?
#define SAVE_LINES 20

typedef struct shape {
  int8_t line[4];
  uint8_t height;
} shape_t;

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

typedef struct state {
  int8_t lines[SAVE_LINES];
  uint32_t height, shape, instruction, rocks;
} state_t;

typedef struct states {
  state_t** state;
  size_t states;
} states_t;

void save_state(states_t* states, int8_t* board, uint32_t y, uint32_t s, uint32_t i, uint32_t r) {
  state_t* state = calloc(1, sizeof(state_t));
  if(y > SAVE_LINES) {
    // only start saving lines after we have enough to fill buffer
    for(size_t l = 0; l < SAVE_LINES; l++) state->lines[l] = board[y - l - 1];
  }
  state->height = y;
  state->shape = s;
  state->instruction = i;
  state->rocks = r;
  states->state[states->states++] = state;

  // printf("Data at %i: height=%i, shape=%i, instruction=%i\n", r, y, s, i);
  // for(size_t l = 0; l < SAVE_LINES; l++) printf(" - line %zu=%i\n", l, state->lines[l]);
  // printf("\n");
}

uint8_t compare_states(state_t* a, state_t* b) {
  if(a->height < SAVE_LINES || b->height < SAVE_LINES) return 0;
  // size_t comp = (a->height < b->height) ? a->height : b->height;
  // comp = (comp < SAVE_LINES) ? comp : SAVE_LINES;
  if(a->shape != b->shape) return 0;
  if(a->instruction != b->instruction) return 0;
  for(size_t i = 0; i < SAVE_LINES; i++) {
    if(a->lines[i] != b->lines[i]) return 0;
  }

  return 1;
}

uint32_t detect_cycle(states_t* states) {
  uint32_t start = 0, end;
  for(size_t i = 0; i < (states->states - 1); i++) {
    uint8_t match = compare_states(states->state[i], states->state[(states->states - 1)]);

    if(match) {
      start = i;
      end = states->states - 1;
      // printf("Cycle - start @ %i, end @ %i\n", start, end);
      return end - start;
    }
  }
  return 0;
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
  int8_t* board = calloc(10000, sizeof(int8_t));
  states_t* states = calloc(1, sizeof(states_t));
  states->state = calloc(10000, sizeof(state_t*));
  states->states = 0;

  uint8_t final_run = 0;
  uint32_t s = 0, height = 0, i = 0, y, at_rest, cycle;
  uint64_t cycle_height = 0;

  for(size_t r = 0; r < 1000000000000; r++) {
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

        if(!final_run) {
          // save current state and see if we can detect a cycle yet...
          save_state(states, board, height, s, i, r);
          cycle = detect_cycle(states);

          if(cycle) {
            // lots of maths to work out how much height/how many rocks a cycle adds,
            // then how many cycles will fit in the remaining rocks needed to simulate,
            // then how much height all those cycles will give us
            uint64_t prior_height = states->state[(states->states - 1 - cycle)]->height;
            uint64_t prior_rocks = states->state[(states->states - 1 - cycle)]->rocks;
            uint64_t dh = states->state[(states->states - 1)]->height - prior_height;
            uint64_t dr = states->state[(states->states - 1)]->rocks - prior_rocks;
            uint64_t cycles = ((1000000000000 - prior_rocks) / (uint64_t) dr) - 1; // -1 as we simulate a cycle before we detect it
            cycle_height = (cycles * dh);

            // after running the cycles we're still ~100 rocks short, so we need to
            // restart the simulation from that point onwards to get the final height
            r += (cycles * dr);
            final_run = 1;
          }
        }
        break;
      }
    }
  }

  free(board);
  free(shape);
  for(size_t i = 0; i < 5; i++) free(shapes[i]);
  for(size_t i = 0; i < states->states; i++) free(states->state[i]);
  free(states->state);
  free(states);

  uint64_t final_height = height + cycle_height;
  printf("Total height after 1 trillion rounds = %llu\n", final_height); // 1572093023267
  return 0;
}
