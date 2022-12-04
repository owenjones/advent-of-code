#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char file[] = "input.txt";
  int l = 1000;
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  for(size_t i = 0; i < l; i++) {

  }

  fclose(fptr);

  printf("");
  return 0;
}
