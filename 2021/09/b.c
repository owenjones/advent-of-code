#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ind(int x, int y) {
  return (100*y) + x;
}

int is_min(int* heightmap, int x, int y) {
  int cell = heightmap[ind(x, y)];

  // for each adjacent cell test if the value is greater (or cell is on an edge)
  int lv = (x == 0 || heightmap[ind(x - 1, y)] > cell);
  int rv = (x == 99 || heightmap[ind(x + 1, y)] > cell);
  int uv = (y == 0 || heightmap[ind(x, y - 1)] > cell);
  int dv = (y == 99 || heightmap[ind(x, y + 1)] > cell);

  return (lv && rv && uv && dv);
}

int get_risk(int* heightmap, int x, int y) {
  return 1 + heightmap[ind(x, y)];
}

int walk(int* heightmap, int* covered, int x, int y) {
  covered[ind(x, y)] = 1; // mark that we've visited the current cell
  int total = 1; // include current cell in count
  int l = x - 1, r = x + 1, u = y - 1, d = y + 1; // identify all adjacent cells
  // start walking in all valid directions...
  if(l >= 0 && covered[ind(l, y)] == 0 && heightmap[ind(l, y)] != 9) total += walk(heightmap, covered, l, y);
  if(r < 100 && covered[ind(r, y)] == 0 && heightmap[ind(r, y)] != 9) total += walk(heightmap, covered, r, y);
  if(u >= 0 && covered[ind(x, u)] == 0 && heightmap[ind(x, u)] != 9) total += walk(heightmap, covered, x, u);
  if(d < 100 && covered[ind(x, d)] == 0 && heightmap[ind(x, d)] != 9) total += walk(heightmap, covered, x, d);
  return total;
}

int walk_basin(int* heightmap, int x, int y) {
  int* covered = (int*) calloc(10000, sizeof(int)); // all possible cells we could cover
  int total = walk(heightmap, covered, x, y); // lets go for a walk...
  free(covered);
  return total;
}

void sort(int inputs[206]) {
  // sort largest first
  int i = 0;
  int temp;
  while(i < 205) {
    if(inputs[i+1] > inputs[i]) {
      temp = inputs[i];
      inputs[i] = inputs[i+1];
      inputs[i+1] = temp;
      i = 0;
    } else {
      i++;
    }
  }
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int* heightmap = (int*) calloc(10000, sizeof(int));
  int i = 0, n;
  while(fscanf(fptr, "%1i", &n) > 0) heightmap[i++] = n;
  fclose(fptr);

  int basins[206]; // slightly cheaty (hardcoding number of min points from part a...)
  int basin = 0;
  for(int y = 0; y < 100; y++) {
    for(int x = 0; x < 100; x++) {
      if(is_min(heightmap, x, y)) basins[basin++] = walk_basin(heightmap, x, y);
    }
  }
  free(heightmap);

  sort(basins);
  int product = basins[0] * basins[1] * basins[2];
  printf("Product of three largest basins: %i\n", product); // 1198704
  return 0;
}
