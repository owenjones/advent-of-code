#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int* window = calloc(1998, sizeof(int));

  int current;
  int last = 0;
  for(size_t i = 0; i < 2000; i++) {
    fscanf(fptr, "%i", &current);

    window[i] += current;
    if(i > 0) window[i-1] += current;
    if(i > 1) window[i-2] += current;
  }
  fclose(fptr);

  int increases = 0;
  for(size_t i = 0; i < 1998; i++) {
    if(i > 0 && window[i] > window[i-1]) increases++;
  }

  printf("Increases: %i\n", increases);
  return 0;
}
