#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct point {
  int32_t x, y;
} point_t;

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  point_t min, max;
  if(fscanf(fptr, "target area: x=%i..%i, y=%i..%i", &min.x, &max.x, &min.y, &max.y) != 4) exit(1);
  fclose(fptr);

  int32_t edge = -min.y - 1;
  uint32_t highest = edge * (edge + 1) / 2;
  
  printf("Highest point reached: %i\n", highest);
  return 0;
}
