#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int c = 0, cubes[2500][3];
  while(fscanf(fptr, "%d,%d,%d\n", &cubes[c][0], &cubes[c][1], &cubes[c][2]) == 3) c++;
  fclose(fptr);

  int adjacent = 0, l, m, n;
  size_t i, j;
  for(i = 0; i < c; i++) {
    for(j = 0; j < c; j++) {
      if(i == j) continue;
  
      l = (abs(cubes[i][0] - cubes[j][0]) == 1) && (cubes[i][1] == cubes[j][1]) && (cubes[i][2] == cubes[j][2]);
  
      m = (abs(cubes[i][1] - cubes[j][1]) == 1) && (cubes[i][0] == cubes[j][0]) && (cubes[i][2] == cubes[j][2]);
  
      n = (abs(cubes[i][2] - cubes[j][2]) == 1) && (cubes[i][1] == cubes[j][1]) && (cubes[i][0] == cubes[j][0]);
  
      if(l ^ m ^ n) {
        adjacent++;
      }
    }
  }

  int total = (6 * c) - adjacent;
  printf("Total surface area: %d (%d adjacent)\n", total, adjacent); // 3412

  return 0;
}
