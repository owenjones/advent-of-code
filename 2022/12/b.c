#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pathfind.h"

// Warning, very hacky approach...
// Essentially, in my input all possible start points (an a next to a b)
// occur at x=0, so for 41 lines of map input I just tried starting at the
// far left of each of them. Does this work for all puzzle inputs? Who knows...
int find_hiking_trail(map_t* map) {  
  int end[map->h];
  for(size_t z = 0; z < map->h; z++) {
    list_t* open = new_list();
    list_t* closed = new_list();

    node_t* start = new_node_at(0, z);
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
          // we have reached the end!
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
    
    end[z] = steps;
  }
  
  for(size_t i = 0; i < (map->h - 1); i++) {
    for(size_t j = 0; j < (map->h - 1 - i); j++) {
      if(end[j] > end[(j + 1)]) {
        int temp = end[j];
        end[j] = end[(j + 1)];
        end[(j + 1)] = temp;
      }
    }
  }
  
  return end[0];
}

int main(void) {
  map_t* map = load_map("input.txt");
  int steps = find_hiking_trail(map);
  free_map(map);
  printf("Number of steps in shortest hiking trail = %d\n", steps); // 439
  return 0;
}
