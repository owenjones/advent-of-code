#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int count;
  int* members;
} population;

void init(population* p) {
  p->count = 0; // total count
  p->members = calloc(1000000, sizeof(int));
}

void disp_population(population* p) {
  for(int i = 0; i < p->count; i++) {
    printf("%i,", p->members[i]);
  }
  printf("\n");
}

void add_to_population(population* p, int age) {
  p->members[p->count] = age;
  p->count++;
}

void tick(population* p) {
  int n = p->count;
  for(int i = 0; i < n; i++) {
    p->members[i]--;
    if(p->members[i] < 0) {
      p->members[i] = 6;
      add_to_population(p, 8);
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

  population* p = calloc(1, sizeof(population));
  init(p);
  
  int n;
  while(fscanf(fptr, "%i,", &n) > 0) {
    add_to_population(p, n);
  }

  fclose(fptr);

  for(int d = 1; d <= 80; d++) {
    tick(p);
    printf("population after %i days = %i\n", d, p->count);
  }

  return 0;
}
