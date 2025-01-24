#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "snailfish.h"

int main(int argc, char **argv)
{
  FILE *fptr;
  if ((fptr = fopen(argv[1], "r")) == NULL)
  {
    debug("Error opening file\n");
    exit(1);
  }

  uint32_t maximum_magnitude = 0;

  char line[256];
  char lines[100][256];
  size_t n_lines = 0;

  while (fgets(line, sizeof(line), fptr))
    strcpy(lines[n_lines++], line);

  fclose(fptr);

  for (size_t i = 0; i < n_lines; i++)
  {
    for (size_t j = 0; j < n_lines; j++)
    {
      if (i == j)
        continue;

      node_t *a = parse_line(lines[i]);
      node_t *b = parse_line(lines[j]);
      node_t *c = parse_line(lines[i]);
      node_t *d = parse_line(lines[j]);

      node_t *x = add_numbers(a, b);
      reduce(x);

      node_t *y = add_numbers(d, c);
      reduce(y);

      uint32_t mag_x = calculate_magnitude(x);
      uint32_t mag_y = calculate_magnitude(y);

      free_tree(x);
      free_tree(y);

      maximum_magnitude = (mag_x > maximum_magnitude) ? mag_x : maximum_magnitude;
      maximum_magnitude = (mag_y > maximum_magnitude) ? mag_y : maximum_magnitude;
    }
  }

  printf("Part 2: %i\n", maximum_magnitude);
  return 0;
}
