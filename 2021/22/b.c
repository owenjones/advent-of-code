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
  if (!(a->x[0] <= b->x[1] && a->x[1] >= b->x[0]))
    return NULL;
  if (!(a->y[0] <= b->y[1] && a->y[1] >= b->y[0]))
    return NULL;
  if (!(a->z[0] <= b->z[1] && a->z[1] >= b->z[0]))
    return NULL;

  int32_t min_x = (a->x[0] > b->x[0]) ? a->x[0] : b->x[0];
  int32_t max_x = (a->x[1] > b->x[1]) ? b->x[1] : a->x[1];
  int32_t min_y = (a->y[0] > b->y[0]) ? a->y[0] : b->y[0];
  int32_t max_y = (a->y[1] > b->y[1]) ? b->y[1] : a->y[1];
  int32_t min_z = (a->z[0] > b->z[0]) ? a->z[0] : b->z[0];
  int32_t max_z = (a->z[1] > b->z[1]) ? b->z[1] : a->z[1];

  int8_t value;
  if (a->value == b->value)
    value = -(a->value);
  else if (a->value == 1 && b->value == -1)
    value = 1;
  else
    value = a->value * b->value;

  return make_cube(min_x, max_x, min_y, max_y, min_z, max_z, value);
}

int64_t volume(cube_t *cube)
{
  return (
      (int64_t)(cube->x[1] - cube->x[0]) *
      (int64_t)(cube->y[1] - cube->y[0]) *
      (int64_t)(cube->z[1] - cube->z[0]) *
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
  if ((fptr = fopen("test_input_2.txt", "r")) == NULL)
  {
    printf("Error opening file\n");
    exit(1);
  }

  list_t *cubes = make_list(), *intersections;
  cube_t *cube, *intersection;
  char action[4];
  int32_t min_x, max_x, min_y, max_y, min_z, max_z;
  int8_t value;
  size_t c, i;
  while (fscanf(fptr, "%s x=%i..%i,y=%i..%i,z=%i..%i\n", action, &min_x, &max_x, &min_y, &max_y, &min_z, &max_z) == 7)
  {
    intersections = make_list();
    value = (strcmp(action, "on") == 0) ? 1 : -1;
    cube = make_cube(min_x, max_x, min_y, max_y, min_z, max_z, value);

    for (c = 0; c < cubes->elements; c++)
    {
      intersection = find_intersection(cube, cubes->element[c]);
      if (intersection != NULL)
        append(intersections, intersection);
    }

    for (i = 0; i < intersections->elements; i++)
    {
      intersection = pop(intersections);
      append(cubes, intersection);
    }

    if (cube->value == 1)
      append(cubes, cube);

    free(intersections->element);
    free(intersections);
  }
  fclose(fptr);

  int64_t count = 0;
  for (size_t c = 0; c < cubes->elements; c++)
    count += volume(cubes->element[c]);

  printf("Total cubes on: %lli\n", count);
  return 0;

  // 1373732982918688 = too high
  // 2134361332352877
}
