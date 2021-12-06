#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int x, y;
} coord;

int coordToIndex(int x, int y) {
  return (x * 1000) + y;
}

int inX(coord* a, coord* b) {
  return (a->x == b->x);
}

int inY(coord* a, coord* b) {
  return (a->y == b->y);
}

void sort(coord* a, coord* b) {
  // sort smallest x/y coord into a
  if((inX(a, b) && (a->y > b->y)) || (inY(a, b) && (a->x > b->x))) {
    coord* temp = a;
    a = b;
    b = temp;
  }
}

void mark(int* seafloor, coord* a, coord* b) {
  printf("Marking from %i,%i to %i,%i\n", a->x, a->y);
  if(inX(a, b)) {
    int x = a->x;
    int diff = b->y - a->y;
    for(int i = 0; i < diff; i++) {
      int y = a->y + i;
      seafloor[coordToIndex(x, y)] += 1;
    }
  } else if(inY(a, b)) {
    int y = a->y;
    int diff = b->x - a->x;
    for(int i = 0; i < diff; i++) {
      int x = a->x + i;
      seafloor[coordToIndex(x, y)] += 1;
    }
  } else {
    exit(1); // line must either be in x or y
  }
}

int finddanger(int* seafloor) {
  int dangers = 0;
  for(int x = 0; x < 1000; x++) {
    for(int y = 0; y < 1000; y++) {
      int ind = coordToIndex(x, y);
      if(seafloor[ind] > 1) dangers++;
    }
  }
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

    // only accept coordinate pairs on the same x/y line
    if(a.x == b.x || a.y == b.y) {
      sort(&a, &b);
      mark(seafloor, &a, &b);
    }
  }

  fclose(fptr);

  int points = finddanger(seafloor);
  printf("Dangerous points: %i\n", points);

  return 0;
}
