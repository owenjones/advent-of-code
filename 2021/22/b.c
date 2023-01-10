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
    uint8_t value = (strcmp(action, "on") == 0);
  }
  fclose(fptr);

  uint64_t count = 0;
  printf("Total cubes on: %llu\n", count);
  return 0;
}
