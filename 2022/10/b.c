#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int cycle = 0, x = 1, dx;
  int value_after[1024];
  
  value_after[0] = x;
  cycle++;
  
  char* input = NULL;
  size_t bufsize = 0;
  while(getline(&input, &bufsize, fptr) != -1) {
    if(strcmp(input, "noop\n") == 0) {
      value_after[cycle] = x;
      cycle++;
    } else if(sscanf(input, "addx %d", &dx) > 0) {
      value_after[cycle] = x;
      x += dx;
      value_after[cycle + 1] = x;
      cycle += 2;
    }
  }
  fclose(fptr);

  int v, p = 0;
  for(size_t i = 0; i < 240; i++) {
    v = value_after[i];
    
    if(p == (v - 1) || p == v || p == (v + 1)) printf("# ");
    else printf(". ");
    
    p++;
    if(((i + 1) % 40) == 0) {
      p = 0;
      printf("\n");
    }
  } // EKRHEPUZ

  return 0;
}
