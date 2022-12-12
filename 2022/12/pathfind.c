#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pathfind.h"

map_t* new_map() {
  map_t* map = calloc(1, sizeof(map_t));
  map->h = 0;
  map->w = 0;
  map->heights = calloc(10000, sizeof(int)); // guess height x width <= 10000
  map->start = NULL;
  map->end = NULL;
  return map;
}

map_t* load_map(char* file) {
  FILE* fptr;
  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  map_t* map = new_map();

  char* input = NULL;
  size_t bufsize = 0;
  int height;
  while(getline(&input, &bufsize, fptr) != -1) {
    if(map->w == 0) map->w = (strlen(input) - 1); // extract width from length of first line
    // origin in top-left, extend down and right in x & y from here (because that's how we read file)
    for(size_t x = 0; x < map->w; x++) {
      height = (int) input[x];
      if(height == 83) {
        // S - start point
        point_t* start = new_point_at(x, map->h);
        map->start = start;
        height = 97; // height of S is a;
      }

      if(height == 69) {
        // E - end point
        point_t* end = new_point_at(x, map->h);
        map->end = end;
        height = 122; // height of E is z;
      }

      map->heights[(map->h * map->w) + x] = (height - 97);
    }
    map->h++;
  }
  free(input);
  fclose(fptr);
  return map;
}

void free_map(map_t* map) {
  free(map->start);
  free(map->end);
  free(map->heights);
  free(map);
}

int is_valid(map_t* map, int x, int y) {
  return (x >= 0) && (x < map->w) && (y >= 0) && (y < map->h);
}

int height_at(map_t* map, int x, int y) {
  return map->heights[(y * map->w) + x];
}

// Points
point_t* new_point() {
  return calloc(1, sizeof(point_t));
}

point_t* new_point_at(int x, int y) {
  point_t* point = new_point();
  point->x = x;
  point->y = y;
  return point;
}

void free_point(point_t* point) {
  free(point);
}

int can_climb_up(map_t* map, point_t* a, point_t* b) {
  // can we move from point a to point b?
  int ha = height_at(map, a->x, a->y);
  int hb = height_at(map, b->x, b->y);

  // can go down any height difference(?) but can only go up difference of 1
  return (hb <= ha) || ((ha + 1) == hb);
}

int manhattan_distance(point_t* a, point_t* b) {
  // calculate the manhattan distance between points a and b
  int dx = abs(b->x - a->x);
  int dy = abs(b->y - a->y);
  return dx + dy;
}

int is_same(point_t* a, point_t* b) {
  return (a->x == b->x) && (a->y == b->y);
}

// Nodes
node_t* new_node() {
  node_t* node = calloc(1, sizeof(node_t));
  node->point = new_point();
  return node;
}

node_t* new_node_at(int x, int y) {
  node_t* node = calloc(1, sizeof(node_t));
  node->point = new_point_at(x, y);
  return node;
}

void free_node(node_t* node) {
  free(node->point);
  free(node);
}

node_t* step_from(node_t* current, int direction) {
  int x, y;
  switch(direction) {
    case 0: // up / north
      x = current->point->x;
      y = (current->point->y - 1);
      break;
    
    case 1: // right / east
      x = (current->point->x + 1);
      y = current->point->y;
      break;
      
    case 2: // down / south
      x = current->point->x;
      y = (current->point->y + 1);
      break;
      
    case 3: // left / west
      x = (current->point->x - 1);
      y = current->point->y;
      break;
  }
  
  return new_node_at(x, y);
}

void set_node_heuristics(node_t* node, node_t* parent, point_t* end) {
  node->g = (parent != NULL) ? (parent->g + 1) : 0;
  node->h = manhattan_distance(node->point, end);
  node->f = node->g + node->h;
}

// Lists
list_t* new_list() {
  // return an empty node list
  list_t* list = calloc(1, sizeof(list_t));
  list->nodes = calloc(1000, sizeof(node_t*));
  return list;
}

void free_list(list_t* list) {
  free(list->nodes);
  free(list);
}

node_t* get_head_node(list_t* list) {
  // take first element off the list
  return remove_node_at(list, 0);
}

node_t* remove_node_at(list_t* list, int i) {
  // remove node at position i from the list and return it
  node_t* h = list->nodes[i];
  for(size_t n = i; n < (list->n - 1); n++) {
    list->nodes[n] = list->nodes[(n + 1)];
  }
  list->n--;
  return h;
}

void append_node(list_t* list, node_t* node) {
  // append new element to end of the list
  list->nodes[list->n++] = node;
}

int find_lowest_node(list_t* list) {
  int lowest = list->nodes[0]->f;
  int ind = 0;
  
  for(size_t i = 0; i < list->n; i++) {
    if(list->nodes[i]->f < lowest){
      lowest = list->nodes[i]->f;
      ind = i;
    }
  }
  
  return ind;
}

int lower_exists_in(list_t* list, node_t* node) {
  for(size_t i = 0; i < list->n; i++) {
    if(is_same(list->nodes[i]->point, node->point)) {
      if(list->nodes[i]->f <= node->f) return 1;
    }
  }
  return 0;
}

// The actual pathfinding
int steps_to_end(map_t* map) {
  // input: map
  // output: number of steps of shortest route
  list_t* open = new_list();
  list_t* closed = new_list();

  node_t* start = new_node_at(map->start->x, map->start->y);
  set_node_heuristics(start, NULL, map->end);
  append_node(open, start);

  int n = open->n, steps = 0;
  node_t *current, *next;
  size_t i;
  while(n > 0 && !steps) {
    current = remove_node_at(open, find_lowest_node(open));

    for(i = 0; i < 4; i++) {
      next = step_from(current, i);
      set_node_heuristics(next, current, map->end);
      
      if(is_same(next->point, map->end)) {
        // we have reached the end!
        steps = next->g;
        free_node(next);
        continue;
      } else if(!is_valid(map, next->point->x, next->point->y)) {
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
    n = open->n;
  }

  free_list(open);
  free_list(closed);

  return steps;
}

int find_hiking_trail(map_t* map) {
  // input: map
  // output: number of steps of shortest route
  
  int end[41];
  for(size_t z = 0; z < 41; z++) {
    list_t* open = new_list();
    list_t* closed = new_list();

    node_t* start = new_node_at(0, z);
    set_node_heuristics(start, NULL, map->end);
    append_node(open, start);

    int n = open->n, steps = 0;
    node_t *current, *next;
    size_t i;
    while(n > 0 && !steps) {
      current = remove_node_at(open, find_lowest_node(open));

      for(i = 0; i < 4; i++) {
        next = step_from(current, i);
        set_node_heuristics(next, current, map->end);
        
        if(is_same(next->point, map->end)) {
          // we have reached the end!
          steps = next->g;
          free_node(next);
          continue;
        } else if(!is_valid(map, next->point->x, next->point->y)) {
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
      n = open->n;
    }

    free_list(open);
    free_list(closed);
    
    end[z] = steps;
  }
  
  for(size_t i = 0; i < 40; i++) {
    for(size_t j = 0; j < (40 - i); j++) {
      if(end[j] > end[(j + 1)]) {
        int temp = end[j];
        end[j] = end[(j + 1)];
        end[(j + 1)] = temp;
      }
    }
  }
  
  return end[0];
}
