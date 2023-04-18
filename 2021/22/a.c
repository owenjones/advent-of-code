#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int main(void)
{
  FILE *fptr;
  if ((fptr = fopen("input.txt", "r")) == NULL)
  {
    printf("Error opening file\n");
    exit(1);
  }

  uint8_t core[200][200][200];
  memset(core, 0, (200 * 200 * 200));

  char action[4];
  int32_t min_x, max_x, min_y, max_y, min_z, max_z, limit = 50;
  while (fscanf(fptr, "%s x=%i..%i,y=%i..%i,z=%i..%i\n", action, &min_x, &max_x, &min_y, &max_y, &min_z, &max_z) == 7)
  {
    if (min_x < -(limit) || max_x > limit || min_y < -(limit) || max_y > limit || min_z < -(limit) || max_z > limit)
    {
      continue;
    }

    uint8_t value = (strcmp(action, "on ") == 0);

    for (int32_t x = min_x; x <= max_x; x++)
    {
      for (int32_t y = min_y; y <= max_y; y++)
      {
        for (int32_t z = min_z; z <= max_z; z++)
        {
          core[x + 50][y + 50][z + 50] = value;
        }
      }
    }
  }
  fclose(fptr);

  uint32_t count = 0;
  for (size_t x = 0; x < 200; x++)
  {
    for (size_t y = 0; y < 200; y++)
    {
      for (size_t z = 0; z < 200; z++)
      {
        count += core[x][y][z];
      }
    }
  }

  printf("Total cubes on: %i\n", count); // 547648
  return 0;
}
