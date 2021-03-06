#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct {
  int x, y;
} coord;

void mark(int* seafloor, coord* a, coord* b) {
  int xraw = b->x - a->x;
  int yraw = b->y - a->y;
  int xdiff = abs(xraw);
  int ydiff = abs(yraw);
  int xchng = (xdiff > 0) ? 1 : 0;
  int ychng = (ydiff > 0) ? 1 : 0;
  int xsign = (xraw > 0) ? 1 : -1;
  int ysign = (yraw > 0) ? 1 : -1;
  int maxdiff = (xdiff > ydiff) ? xdiff : ydiff;
  
  for(int i = 0; i <= maxdiff; i++) {
    int x = a->x + (i * xsign * xchng);
    int y = a->y + (i * ysign * ychng);
    seafloor[(y * 1000) + x] += 1;
  }
}

int finddanger(int* seafloor) {
  int dangers = 0;
  for(int i = 0; i < (1000 * 1000); i++)
    if(seafloor[i] > 1) dangers++;
  
  return dangers;
}

int main(void) {
  char file[] = "input.txt";
  int l = 500;
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int* seafloor = (int*) calloc(1000 * 1000, sizeof(int));
  // assume seafloor only extends to 1000 in x & y?

  coord a, b;
  for(size_t i = 0; i < l; i++) {
    fscanf(fptr, "%i,%i -> %i,%i", &a.x, &a.y, &b.x, &b.y);
    mark(seafloor, &a, &b);
  }

  fclose(fptr);

  int points = finddanger(seafloor);
  printf("Dangerous points: %i\n", points);

  return 0;
}
