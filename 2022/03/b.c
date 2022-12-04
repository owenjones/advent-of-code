#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int letter_value(char letter) {
  int v = (uint8_t) letter;
  if(v < 91) return (v - 38);
  else return (v - 96);
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  
  int total = 0;
  char* input = calloc(100, sizeof(char));
  int len, split, value;
  size_t i, j, gcount = 0;
  int* common = calloc(52, sizeof(int));
  int* matched;

  while(fscanf(fptr, "%s", input) > 0) {
    matched = calloc(52, sizeof(int));
    len = strlen(input);
    
    for(i = 0; i < len; i++) {
      value = letter_value(input[i]);
      
      if(matched[(value - 1)] == 0) {
        matched[(value -1)] = 1;
        common[(value - 1)] += 1;
      }
    }
    
    gcount++;
    if(gcount == 3) {
      for(i = 0; i < 52; i++) {
        if(common[i] == 3) total += (i + 1);
      }
      free(common);
      common = calloc(52, sizeof(int));
      gcount = 0;
    }
  }
  free(input);

  fclose(fptr);

  printf("Total of priority items: %d\n", total); // 2434
  return 0;
}
