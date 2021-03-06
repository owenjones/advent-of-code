#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

void dispgrid(uint8_t* g) {
  for(size_t i = 0; i < 100; i++) {
    printf("%i", g[i]);
    if((i + 1) % 10 == 0) printf("\n");
  }
  printf("\n");
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  uint8_t* grid = calloc(104, sizeof(uint8_t));
  size_t i = 0;
  while(fscanf(fptr, "%1hhi", &grid[i]) > 0) i++;
  fclose(fptr);

  // for 100 steps:
  // * step through all positions and increase by 1
  // * step through all positions and check if there's a flash - mark as flashed,
  //   increase all surrounding positions by 1 and restart from beginning of grid
  // * once we've made it to end of grid, reset all flashed to 0 and move to next step

  uint32_t flashes = 0;
  uint8_t* flashed = calloc(100, sizeof(uint8_t));
  size_t s;

  for(s = 0; s < 1000; s++) {
    for(size_t i = 0; i < 100; i++) grid[i]++;

    size_t i = 0;
    while(i < 100) {
      if(grid[i] > 9 && flashed[i] == 0) {
        // FLASH (ah ahhh)
        flashes++;
        flashed[i] = 1;

        int x = i % 10;
        int y = i / 10;

        // work around adjacent cells clockwise from 12 o'clock.
        if(y - 1 >= 0) grid[i - 10]++;
        if(y - 1 >= 0 && x + 1 < 10) grid[i - 10 + 1]++;
        if(x + 1 < 10) grid[i + 1]++;
        if(y + 1 < 10 && x + 1 < 10) grid[i + 10 + 1]++;
        if(y + 1 < 10) grid[i + 10]++;
        if(y + 1 < 10 && x - 1 >= 0) grid[i + 10 - 1]++;
        if(x - 1 >= 0) grid[i - 1]++;
        if(y - 1 >= 0 && x - 1 >= 0) grid[i - 10 - 1]++;

        // go back to start of the grid and check for new flashes
        i = 0;
      } else {
        i++;
      }
    }

    int sum = 0;
    for(size_t i = 0; i < 100; i++) {
      sum += (int) flashed[i];
      grid[i] = flashed[i] ? 0 : grid[i];
      flashed[i] = 0;
    }

    if(sum == 100) {
      printf("Flashed on step: %zu\n", (s+1));
      free(grid);
      free(flashed);
      return 0;
    }
  }

  free(grid);
  free(flashed);

  printf("No flash sync...\n"); // 440
  return 1;
}
