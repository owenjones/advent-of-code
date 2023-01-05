#include "state.h"

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
}

uint8_t compare_states(state_t* a, state_t* b) {
  if(a->height < SAVE_LINES || b->height < SAVE_LINES) return 0;
  if(a->shape != b->shape) return 0;
  if(a->instruction != b->instruction) return 0;
  for(size_t i = 0; i < SAVE_LINES; i++) {
    if(a->lines[i] != b->lines[i]) return 0;
  }

  return 1;
}

uint32_t detect_cycle(states_t* states) {
  for(size_t i = 0; i < (states->states - 1); i++) {
    uint8_t match = compare_states(states->state[i], states->state[(states->states - 1)]);
    if(match) return (states->states - 1) - i;
  }
  return 0;
}
