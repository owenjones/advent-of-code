#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int main(void) {
  FILE* fptr;
  if((fptr = fopen("test_input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  fclose(fptr);

  printf("");
  return 0;
}
