#include "burrows.h"
#include "queue.h"

/*
= rough solution =
this problem is essentially pathfinding, with a changing state and tight constraints on where moves
can be made

at each step we need to generate all the possible outcome moves, calculate their
cost and add them to the queue, then pop the lowest cost state and repeat

once we have all amphipods in the correct room then that should be the lowest cost way of organising them?
*/

uint32_t find_optimal_organisation(burrow_t *start)
{
  queue_t *queue = qinsert(NULL, start);
  burrow_t *burrow, *state;
  while ((burrow = qpop(&queue)))
  {
    if (is_organised(burrow))
    {
      qfree(queue);
      return burrow->cost;
    }

    // first check amphipods in hallway: can only move into correct room & only if free -
    // A to 00/01, B to 10/11, C to 20/21, D to 30/31
    // this will only produce at most 1 more state?
    for (size_t i = 0; i < 11; i++)
    {
      if (burrow->hallway[i] != '.')
      {
        char amphipod = burrow->hallway[i];
        uint8_t t = target_room(amphipod);
        uint8_t l;

        if (burrow->rooms[t][0] == '.' && burrow->rooms[t][1] == '.')
          l = 1;
        else if (burrow->rooms[t][0] == '.')
          l = 0;
        else
          continue;

        if (can_move_from_hallway(burrow, i, t))
        {
          state = malloc(sizeof(burrow_t));
          memcpy(state, burrow, sizeof(burrow_t));
          state->hallway[i] = '.';
          state->rooms[t][l] = amphipod;
          state->cost += movement_cost(amphipod, 0);
          qinsert(queue, state);
        }
      }
    }

    // for the four rooms -- only deal with which level is empty
    // check which hallway spaces we can access and generate an output state
    // for all that are free

    // moving directly from a room to another room is possible but
    // breaking this up into two stages (room -> hallway, then hallway -> room) will be
    // less efficient(?), but movement costs will be the same and logic is simpler

    // will be state branches where no moves are possible?
    // we also need to track minimum cost taken to reach a specific state (so if we get
    // to same state with a higher cost we can drop it as we know it'll be less efficient)
  }

  return 0;
}

int main(int argc, char **argv)
{
  burrow_t burrow = load_burrow(argv[1]);
  uint32_t optimal = find_optimal_organisation(&burrow);
  printf("Part 1: %i\n", optimal);
  return 0;
}
