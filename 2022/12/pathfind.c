#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pathfind.h"

map_t* load_map(char* file) {
  FILE* fptr;
  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  map_t* map = calloc(1, sizeof(map_t));
  map->h = 0;
  map->w = 0;
  map->heights = calloc(10000, sizeof(int)); // guess height x width <= 10000
  map->start = NULL;
  map->end = NULL;

  char* input = NULL;
  size_t bufsize = 0;
  int height;
  while(getline(&input, &bufsize, fptr) != -1) {
    if(map->w == 0) map->w = (strlen(input) - 1); // extract width from length of first line
    
    // origin in top-left, extend down and right in x & y from here
    // (because that's how we read file in)
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

int can_climb_up(map_t* map, point_t* a, point_t* b) {
  // can we move from point a to point b?
  int ha = height_at(map, a);
  int hb = height_at(map, b);

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

int is_valid(map_t* map, point_t* p) {
  return (p->x >= 0) && (p->x < map->w) && (p->y >= 0) && (p->y < map->h);
}

int height_at(map_t* map, point_t* p) {
  return map->heights[(p->y * map->w) + p->x];
}

// Nodes
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

void update_node_heuristics(node_t* node, node_t* parent, point_t* end) {
  node->g = (parent != NULL) ? (parent->g + 1) : 0;
  node->h = manhattan_distance(node->point, end);
  node->f = node->g + node->h;
}

// Lists
list_t* new_list() {
  // return an empty node list
  list_t* list = calloc(1, sizeof(list_t));
  list->nodes = calloc(10000, sizeof(node_t*));
  return list;
}

void free_list(list_t* list) {
  for(size_t n = 0; n < list->n; n++) free_node(list->nodes[n]);
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
