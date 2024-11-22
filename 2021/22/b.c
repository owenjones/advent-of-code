#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>

typedef struct cube
{
  int8_t value;
  int32_t x[2], y[2], z[2];
} cube_t;

typedef struct list
{
  uint32_t elements;
  cube_t **element;
} list_t;

cube_t *make_cube(int32_t min_x, int32_t max_x, int32_t min_y, int32_t max_y, int32_t min_z, int32_t max_z, int8_t value)
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

cube_t *find_intersection(cube_t *a, cube_t *b)
{
  int32_t min_x = (a->x[0] > b->x[0]) ? a->x[0] : b->x[0];
  int32_t max_x = (a->x[1] > b->x[1]) ? b->x[1] : a->x[1];
  int32_t min_y = (a->y[0] > b->y[0]) ? a->y[0] : b->y[0];
  int32_t max_y = (a->y[1] > b->y[1]) ? b->y[1] : a->y[1];
  int32_t min_z = (a->z[0] > b->z[0]) ? a->z[0] : b->z[0];
  int32_t max_z = (a->z[1] > b->z[1]) ? b->z[1] : a->z[1];

  if ((min_x > max_x) || (min_y > max_y) || (min_z > max_z))
    return NULL;

  return make_cube(min_x, max_x, min_y, max_y, min_z, max_z, -(b->value));
}

int64_t volume(cube_t *cube)
{
  return (
      (int64_t)(cube->x[1] - cube->x[0] + 1) *
      (int64_t)(cube->y[1] - cube->y[0] + 1) *
      (int64_t)(cube->z[1] - cube->z[0] + 1) *
      (int64_t)cube->value);
}

list_t *make_list()
{
  list_t *list = calloc(1, sizeof(list_t));
  list->elements = 0;
  list->element = calloc(INT_MAX, sizeof(cube_t *));
  return list;
}

cube_t *pop(list_t *list)
{
  cube_t *head = list->element[0];
  for (size_t i = 0; i < (list->elements - 1); i++)
    list->element[i] = list->element[(i + 1)];
  list->elements--;
  return head;
}

void append(list_t *list, cube_t *cube)
{
  list->element[list->elements++] = cube;
}

int main(void)
{
  FILE *fptr;
  if ((fptr = fopen("input.txt", "r")) == NULL)
  {
    printf("Error opening file\n");
    exit(1);
  }

  list_t *cubes = make_list();
  cube_t *cube;
  char action[4];
  int32_t min_x, max_x, min_y, max_y, min_z, max_z;
  int8_t value;
  while (fscanf(fptr, "%s x=%i..%i,y=%i..%i,z=%i..%i\n", action, &min_x, &max_x, &min_y, &max_y, &min_z, &max_z) == 7)
  {
    value = (strcmp(action, "on") == 0) ? 1 : 0;
    cube = make_cube(min_x, max_x, min_y, max_y, min_z, max_z, value);
    append(cubes, cube);
  }
  fclose(fptr);

  list_t *cores = make_list();

  list_t *add;
  cube_t *intersection;
  size_t i, j;
  for (i = 0; i < cubes->elements; i++)
  {
    add = make_list();

    if (cubes->element[i]->value == 1)
      append(add, cubes->element[i]);

    for (j = 0; j < cores->elements; j++)
    {
      intersection = find_intersection(cubes->element[i], cores->element[j]);
      if (intersection != NULL)
        append(add, intersection);
    }

    for (j = 0; j < add->elements; j++)
    {
      append(cores, add->element[j]);
    }

    free(add);
  }

  int64_t count = 0;
  for (size_t c = 0; c < cores->elements; c++)
    count += volume(cores->element[c]);

  printf("Total cubes on: %li\n", count);
  return 0;
}
