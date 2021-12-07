#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int main(void) {
  char file[] = "input.txt";
  int l = 1000;
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  uint32_t crabs[1000];
  int n, i = 0;
  while(fscanf(fptr, "%i,", &n) > 0) {
    crabs[i] = n;
    i++;
  }
  fclose(fptr);

  uint32_t costs[1000];
  uint32_t cost;
  for(size_t i = 0; i < 1000; i++) {
    cost = 0;
    for(size_t j = 0; j < 1000; j++) {
      cost += abs(i - crabs[j]);
    }
    costs[i] = cost;
  }

  uint32_t lowest = 10000000;
  for(size_t i = 0; i < 1000; i++) {
    if(costs[i] < lowest) lowest = costs[i];
  }

  printf("Lowest cost: %i\n", lowest); // 344138
  return 0;
}
