#include <stdio.h>
#include <stdlib.h>

#define YLINE 2000000
// #define YLINE 20

struct range {
  int min, max;
};

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  struct range ranges[25];
  int ax, ay, bx, by, d, e, f, r = 0;
  while(fscanf(fptr, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n", &ax, &ay, &bx, &by) == 4) {
    d = abs(ax - bx) + abs(ay - by);
    
    if(((ay - d) <= YLINE) && ((ay + d) >= YLINE)) {
      e = abs(ay - YLINE);
      f = d - e;
      
      struct range range;
      range.min = ax - f;
      range.max = ax + f;
      ranges[r++] = range;
    }
  }
  fclose(fptr);

  if(r == 0) {
    printf("Error - no points on the line\n");
    exit(1);
  }

  // sort ranges by min value (ascending)
  for(size_t i = 0; i < (r - 1); i++) {
    for(size_t j = 0; j < (r - i - 1); j++) {
      if(ranges[j].min > ranges[j+1].min) {
        struct range temp = ranges[j];
        ranges[j] = ranges[j+1];
        ranges[j+1] = temp;
      }
    }
  }

  // merge overlapping ranges
  for(size_t i = 0; i < (r - 1); i++) {
    if(ranges[i].max > ranges[i+1].min && ranges[i].max < ranges[i+1].max) {
      ranges[i].max = ranges[i+1].max;
      for(size_t j = (i + 1); j < (r - 1); j++) {
        ranges[j] = ranges[j+1];
      }
      r--;
      i--;
    } else if(ranges[i].min <= ranges[i+1].min && ranges[i].max >= ranges[i+1].max) {
      for(size_t j = (i + 1); j < (r - 1); j++) {
        ranges[j] = ranges[j+1];
      }
      r--;
      i--;
    }
  }

  int total = 0;
  for(size_t i = 0; i < r; i++) {
    total += abs(ranges[i].max - ranges[i].min);
  }

  printf("Total covered positions: %d\n", total); // 4951427
  return 0;
}
