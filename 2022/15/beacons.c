#include "beacons.h"

point_t* point() {
  return calloc(1, sizeof(point_t));
}

point_t* point_at(uint32_t x, uint32_t y) {
  point_t* p = point();
  p->x = x;
  p->y = y;
  return p;
}

int distance(point_t* a, point_t* b) {
  int dx = abs(b->x - a->x);
  int dy = abs(b->y - a->y);
  return dx + dy;
}


list_t* list() {
  // return an empty node list
  list_t* list = calloc(1, sizeof(list_t));
  list->point = calloc(10000, sizeof(point_t*));
  return list;
}

void free_list(list_t* list) {
  for(size_t n = 0; n < list->len; n++) free(list->point[n]);
  free(list->point);
  free(list);
}

point_t* get_head(list_t* list) {
  // take first element off the list
  return remove_at(list, 0);
}

point_t* remove_at(list_t* list, int i) {
  // remove node at position i from the list and return it
  point_t* h = list->point[i];
  for(size_t n = i; n < (list->len - 1); n++) {
    list->point[n] = list->point[(n + 1)];
  }
  list->len--;
  return h;
}

void append(list_t* list, point_t* point) {
  // append new element to end of the list
  list->point[list->len++] = point;
}

grid_t* grid(int offset) {
  grid_t* g = calloc(1, sizeof(grid_t));
  g->offset = offset;
  g->dim = (2 * offset);
  g->values = calloc((g->dim * g->dim), sizeof(int));
  return g;
}

uint32_t c2ind(grid_t* grid, uint32_t x, uint32_t y) {
  return (y * grid->dim) + x;
}
