#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


int ind(int x, int y) {
  return x + (10 * y);
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  uint8_t* grid = (uint8_t*) calloc(100, sizeof(uint8_t));
  size_t i = 0;
  uint8_t n;
  while(fscanf(fptr, "%1s", &n) > 0) grid[i++] = n;
  fclose(fptr);

  uint32_t flashes = 0;
  for(size_t s = 0; s < 100; s++) {
    
  }

  printf("Total number of flashes: %i\n", flashes);
  free(grid);
  return 0;
}
