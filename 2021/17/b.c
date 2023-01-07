#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct point {
  int32_t x, y;
  struct velocity {
    int32_t x, y;
  } velocity;
  uint8_t has_hit;
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

  point_t probes[100000];
  size_t p = 0;
  for(size_t x = 0; x <= max.x; x++) {
    for(int16_t y = -200; y <= 200; y++) {
      probes[p].x = 0;
      probes[p].y = 0;
      probes[p].velocity.x = x;
      probes[p].velocity.y = y;
      probes[p].has_hit = 0;
      p++;
    }
  }

  for(size_t i = 0; i < 1000; i++) {
    for(size_t j = 0; j < p; j++) {
      probes[j].x += probes[j].velocity.x;
      probes[j].y += probes[j].velocity.y;
      int32_t x_change = (probes[j].velocity.x == 0) ? 0 : (probes[j].velocity.x > 0) ? -1 : 1;
      probes[j].velocity.x += x_change;
      probes[j].velocity.y -= 1;

      if(
        probes[j].x >= min.x &&
        probes[j].x <= max.x &&
        probes[j].y >= min.y &&
        probes[j].y <= max.y
      ) {
        probes[j].has_hit = 1;
      }
    }
  }

  uint32_t hits = 0;
  for(size_t i = 0; i < p; i++) hits += probes[i].has_hit;

  printf("Possible probes that can hit the target area: %i\n", hits); // 2555
  return 0;
}
