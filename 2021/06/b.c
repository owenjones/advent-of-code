#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  uint64_t fish[9];
  for(size_t i = 0; i < 9; i++) fish[i] = 0;

  uint8_t n;
  while(fscanf(fptr, "%i,", &n) > 0) fish[n]++;

  fclose(fptr);

  uint64_t c = 0;
  for(size_t i = 0; i < 9; i++) c += fish[i];
  printf("population after 0 days = %i\n", c);

  uint64_t recycle;
  size_t d;
  for(d = 0; d < 256; d++) {
    recycle = fish[0];
    for(size_t i = 0; i < 8; i++) fish[i] = fish[i+1];
    fish[6] += recycle;
    fish[8] = recycle;
  }

  c = 0;
  for(size_t i = 0; i < 9; i++) c += fish[i];
  printf("population after %i days = %llu\n", d, c);

  return 0;
}
