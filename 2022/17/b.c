#include "totris.h"
#include "state.h"

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
            uint16_t prior_height = states->state[(states->states - 1 - cycle)]->height;
            uint16_t prior_rocks = states->state[(states->states - 1 - cycle)]->rocks;
            uint16_t dh = states->state[(states->states - 1)]->height - prior_height;
            uint16_t dr = states->state[(states->states - 1)]->rocks - prior_rocks;
            uint64_t cycles = ((1000000000000 - prior_rocks) / dr) - 1; // -1 as we simulate a cycle before we detect it
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
