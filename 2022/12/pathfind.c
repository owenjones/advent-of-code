#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pathfind.h"

map_t* new_map() {
  point_t* start = new_point();
  point_t* end = new_point();

  map_t* map = calloc(1, sizeof(map_t));
  map->h = 0;
  map->w = 0;
  map->heights = calloc(10000, sizeof(int)); // guess height x width <= 10000
  map->start = start;
  map->end = end;
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
        map->start->x = x;
        map->start->y = map->h;
        height = 97; // height of S is a;
      }

      if(height == 69) {
        // E - end point
        map->end->x = x;
        map->end->y = map->h;
        height = 122; // height of E is z;
      }

      map->heights[(map->h * map->w) + x] = (height - 96);
    }
    map->h++;
  }

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

int can_climb(map_t* map, point_t* a, point_t* b) {
  // can we move from point a to point b?
  int ha = height_at(map, a->x, a->y);
  int hb = height_at(map, b->x, b->y);

  // can go down any height difference(?) but can only go up difference of 1
  return(hb < ha) || ((ha + 1) == hb);
}

int manhattan_distance(point_t* a, point_t* b) {
  // calculate the manhattan distance between points a and b
  int dx = abs(b->x - a->x);
  int dy = abs(b->y - a->y);
  return dx + dy;
}

int is_end(point_t* a, point_t* b) {
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

void set_node_heuristics(node_t* node, node_t* parent, point_t* end) {
  node->h = manhattan_distance(node->point, end);
  node->g = (parent != NULL) ? parent->g + 1 : 0;
  node->f = node->g + node->h;
  printf("Node heuristics: h=%d, g=%d, f=%d\n", node->h, node->g, node->f);
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

node_t* head_node(list_t* list) {
  // take first element off the list
  node_t* h = list->nodes[0];
  for(size_t n = 0; n < (list->n - 1); n++) {
    list->nodes[n] = list->nodes[(n + 1)];
  }
  list->n--;
  return h;
}

void append_node(list_t* list, node_t* node) {
  // append new element to end of the list
  list->nodes[list->n++] = node;
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

  int n = open->n;
  node_t* current;
  while(n > 0) {
    current = get_head_node(open);

    // generate points in cardinal directions
    // - check if point is the end
    // - check if point is valid (in bounds)
    // - check we can climb to this new point
    // 

    n = open->n;
  }

  free_list(open);
  free_list(closed);
  return 0;
}
