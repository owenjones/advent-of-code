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
  fgets(line, sizeof(line), fptr);

  node_t *next, *root = parse_line(line);
  while (fgets(line, sizeof(line), fptr))
  {
    next = parse_line(line);
    root = add_numbers(root, next);
    reduce(root);
  }
  fclose(fptr);

  uint32_t result = calculate_magnitude(root);
  printf("Part 1: %i\n", result);
  free_tree(root);
  return 0;
}
