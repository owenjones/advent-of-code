#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int increases = 0;
  int current;
  int last = 0;
  for(size_t i = 0; i < 2000; i++) {
    fscanf(fptr, "%i", &current);

    if(i > 0 && current > last) {
      increases++;
    }

    last = current;
  }

  fclose(fptr);
  printf("Increases: %i\n", increases);
  return 0;
}
