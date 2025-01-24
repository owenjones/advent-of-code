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

  node_t *root = malloc(sizeof(node_t));
  init_node(root);
  parse_line(root, line);

  node_t *next;
  while (fgets(line, sizeof(line), fptr))
  {
    debug("  ");
    to_string(root);
    next = malloc(sizeof(node_t));
    init_node(next);
    parse_line(next, line);
    debug("+ ");
    to_string(next);
    root = add_numbers(root, next);
    reduce(root);
    debug("= ");
    to_string(root);
    debug("\n");
  }
  fclose(fptr);

  uint32_t result = calculate_magnitude(root);
  printf("Part 1: %i\n", result);
  return 0;
}
