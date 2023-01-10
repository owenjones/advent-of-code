#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// probably going to need this: https://en.wikipedia.org/wiki/Inclusion%E2%80%93exclusion_principle

typedef struct cube
{
  uint8_t value;
  int32_t x[2], y[2], z[2];
} cube_t;

typedef struct list
{
  uint32_t elements;
  cube_t **element;
} list_t;

cube_t *make_cube(int32_t min_x, int32_t max_x, int32_t min_y, int32_t max_y, int32_t min_z, int32_t max_z, uint8_t value)
{
  cube_t *cube = calloc(1, sizeof(cube_t));
  cube->x[0] = min_x;
  cube->x[1] = max_x;
  cube->y[0] = min_y;
  cube->y[1] = max_y;
  cube->z[0] = min_z;
  cube->z[1] = max_z;
  cube->value = value;
  return cube;
}

cube_t *intersection(cube_t *a, cube_t *b)
{
  return NULL;
}

uint64_t volume(cube_t *cube)
{
  return (
      (uint64_t)(cube->x[1] - cube->x[0]) *
      (uint64_t)(cube->y[1] - cube->y[0]) *
      (uint64_t)(cube->z[1] - cube->z[0]) *
      (uint64_t)cube->value);
}

list_t *make_list(uint32_t elements)
{
  list_t *list = calloc(1, sizeof(list_t));
  list->elements = 0;
  list->element = calloc(elements, sizeof(cube_t *));
}

int main(void)
{
  FILE *fptr;
  if ((fptr = fopen("input.txt", "r")) == NULL)
  {
    printf("Error opening file\n");
    exit(1);
  }

  char action[4];
  int32_t min_x, max_x, min_y, max_y, min_z, max_z;
  while (fscanf(fptr, "%s x=%i..%i,y=%i..%i,z=%i..%i\n", &action, &min_x, &max_x, &min_y, &max_y, &min_z, &max_z) == 7)
  {
    cube_t *cube = make_cube(min_x, max_x, min_y, max_y, min_z, max_z, (strcmp(action, "on") == 0));
    printf("test volume=%llu\n", volume(cube));
  }
  fclose(fptr);

  uint64_t count = 0;
  printf("Total cubes on: %llu\n", count);
  return 0;
}
