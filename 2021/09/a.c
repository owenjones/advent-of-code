#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ind(int x, int y) {
  return (100*y) + x;
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int* heightmap = calloc(100 * 100, sizeof(int));
  int i = 0, n;
  while(fscanf(fptr, "%1i", &n) > 0) heightmap[i++] = n;
  fclose(fptr);

  int risk = 0;
  for(int y = 0; y < 100; y++) {
    for(int x = 0; x < 100; x++) {
      int cell = heightmap[ind(x, y)];
      
      // for each adjacent cell test if the value is greater (or cell is on an edge)
      int lv = (x == 0 || heightmap[ind(x - 1, y)] > cell);
      int rv = (x == 99 || heightmap[ind(x + 1, y)] > cell);
      int uv = (y == 0 || heightmap[ind(x, y - 1)] > cell);
      int dv = (y == 99 || heightmap[ind(x, y + 1)] > cell);
      
      if(lv && rv && uv && dv) risk += 1 + cell;
    }
  }
  
  printf("Risk level: %i\n", risk); // 423
  return 0;
}
