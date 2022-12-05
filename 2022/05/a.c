#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cratemover.h"

int main(void) {
  FILE* fptr;
  if((fptr = fopen(INPUT, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  crate_t* stacks[STACKS];
  init_stacks(stacks);
  fill_stacks(stacks, fptr);
  
  int n, from, to;
  while(fscanf(fptr, "move %i from %i to %i\n", &n, &from, &to) > 0) {
    print_stacks(stacks);
    move_crates_individually(stacks, n, from, to);
  }
  fclose(fptr);
  
  print_stacks(stacks);
  get_top_crates(stacks); // GRTSWNJHH
  free_stacks(stacks);
  
  return 0;
}
