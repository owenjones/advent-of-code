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

  char line[256];
  char lines[100][256];
  size_t n_lines = 0;

  while (fgets(line, sizeof(line), fptr))
    strcpy(lines[n_lines++], line);

  fclose(fptr);

  node_t *a, *b, *c, *d, *x, *y;
  uint32_t mag_x, mag_y, maximum_magnitude = 0;
  for (size_t i = 0; i < n_lines; i++)
  {
    for (size_t j = 0; j < n_lines; j++)
    {
      if (i == j)
        continue;

      a = parse_line(lines[i]);
      b = parse_line(lines[j]);
      c = parse_line(lines[i]);
      d = parse_line(lines[j]);

      x = add_numbers(a, b);
      reduce(x);

      y = add_numbers(d, c);
      reduce(y);

      mag_x = calculate_magnitude(x);
      mag_y = calculate_magnitude(y);

      free_tree(x);
      free_tree(y);

      maximum_magnitude = (mag_x > maximum_magnitude) ? mag_x : maximum_magnitude;
      maximum_magnitude = (mag_y > maximum_magnitude) ? mag_y : maximum_magnitude;
    }
  }

  printf("Part 2: %i\n", maximum_magnitude);
  return 0;
}
