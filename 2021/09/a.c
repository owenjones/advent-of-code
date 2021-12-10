#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ind(int x, int y) {
  return (100*y) + x;
}

int is_min(int* heightmap, int x, int y) {
  int cell = heightmap[ind(x, y)];

  // for each adjacent cell test if the value is greater (or cell is on an edge)
  int l = (x == 0 || heightmap[ind(x - 1, y)] > cell);
  int r = (x == 99 || heightmap[ind(x + 1, y)] > cell);
  int u = (y == 0 || heightmap[ind(x, y - 1)] > cell);
  int d = (y == 99 || heightmap[ind(x, y + 1)] > cell);

  return (l && r && u && d);
}

int get_risk(int* heightmap, int x, int y) {
  return 1 + heightmap[ind(x, y)];
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
      if(is_min(heightmap, x, y)) risk += get_risk(heightmap, x, y);
    }
  }

  printf("Risk level: %i\n", risk); // 423
  return 0;
}
