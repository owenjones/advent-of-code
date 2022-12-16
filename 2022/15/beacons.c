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

int distance(uint32_t ax, uint32_t ay, uint32_t bx, uint32_t by) {
  int dx = abs((int) (bx - ax));
  int dy = abs((int) (by - ay));
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

grid_t* grid(uint64_t dim, uint32_t offset) {
  grid_t* g = calloc(1, sizeof(grid_t));
  g->offset = offset;
  g->dim = dim;
  g->values = calloc((dim * dim), sizeof(uint8_t));
  return g;
}

uint32_t c2ind(grid_t* grid, uint32_t x, uint32_t y) {
  return (y * grid->dim) + x;
}

void mark_point(grid_t* map, uint32_t x, uint32_t y) {
  if(x >= 0 && x < map->dim && y >= 0 && y < map->dim) {
    map->values[c2ind(map, x, y)] = 1;
  }
}
