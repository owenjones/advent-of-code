#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pathfind.h"

// Pathfinding (sketchy implementation of A* algorithm)
int steps_to_end(map_t* map) {
  list_t* open = new_list();
  list_t* closed = new_list();

  node_t* start = new_node_at(map->start->x, map->start->y);
  update_node_heuristics(start, NULL, map->end);
  append_node(open, start);

  int steps = 0;
  node_t *current, *next;
  size_t i;
  while(open->n > 0 && !steps) {
    current = remove_node_at(open, find_lowest_node(open));

    for(i = 0; i < 4; i++) {
      next = step_from(current, i);
      update_node_heuristics(next, current, map->end);
      
      if(is_same(next->point, map->end)) {
        steps = next->g;
        free_node(next);
      } else if(!is_valid(map, next->point)) {
        free_node(next);
      } else if(!can_climb_up(map, current->point, next->point)) {
        free_node(next);
      } else if(!lower_exists_in(open, next) && !lower_exists_in(closed, next)) {
        append_node(open, next);
      } else {
        free_node(next);
      }
    }
    append_node(closed, current);
  }

  free_list(open);
  free_list(closed);

  return steps;
}

int main(void) {
  map_t* map = load_map("input.txt");
  int steps = steps_to_end(map);
  free_map(map);
  printf("Number of steps in shortest route from start to end = %d\n", steps); // 440
  return 0;
}
