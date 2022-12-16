#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

typedef struct point {
  uint32_t x, y;
} point_t;

typedef struct plist {
  int len;
  point_t** point;
} list_t;

typedef struct grid {
  uint32_t offset;
  uint64_t dim;
  uint8_t* values;
} grid_t;

point_t* point();
point_t* point_at(uint32_t, uint32_t);
int distance(uint32_t, uint32_t, uint32_t, uint32_t);

list_t* list();
void free_list(list_t*);
point_t* get_head(list_t*);
point_t* remove_at(list_t*, int);
void append(list_t*, point_t*);

grid_t* grid(uint64_t, uint32_t);
uint32_t c2ind(grid_t*, uint32_t, uint32_t);
void mark_point(grid_t*, uint32_t, uint32_t);
