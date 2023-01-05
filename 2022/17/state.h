#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// how many lines of the board do we save in state?
#define SAVE_LINES 20

typedef struct state {
  int8_t lines[SAVE_LINES];
  uint32_t height, shape, instruction, rocks;
} state_t;

typedef struct states {
  state_t** state;
  size_t states;
} states_t;

void save_state(states_t*, int8_t*, uint32_t, uint32_t, uint32_t, uint32_t);
uint8_t compare_states(state_t*, state_t*);
uint32_t detect_cycle(states_t*);
