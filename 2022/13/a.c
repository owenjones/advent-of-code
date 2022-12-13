#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int in_order(char* left, char* right) {
  
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("test_input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  char *left = NULL, *right = NULL;
  size_t bufl = 0, bufr = 0;
  int cl, cr, pair = 1, total = 0;
  while(1) {
    cl = getline(&left, &bufl, fptr);
    cr = getline(&right, &bufr, fptr);
    if((cl > 0) && (cr > 0)) {
      if(in_order(left, right)) total += pair;
      getline(&left, &bufl, fptr); // clear the newline after a pair
      pair++;
    } else {
      break;
    }
  }
  fclose(fptr);
  free(left);
  free(right);

  printf("Sum of indices already in order: %d\n", total);
  return 0;
}
