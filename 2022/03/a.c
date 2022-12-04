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
  size_t i, j;
  int* matched = NULL;

  while(fscanf(fptr, "%s", input) > 0) {
    matched = calloc(52, sizeof(int));
    len = strlen(input);
    split = len / 2;
    
    printf("%s => %d (%d)\n", input, len, split);
    
    for(i = 0; i < split; i++) {
      for(j = split; j < len; j++) {
        //printf("%c - %c\n", input[i], input[j]);
        if(input[i] == input[j]) {
          value = letter_value(input[i]);
          
          if(matched[(value - 1)] == 0) { // only match once per line
            matched[(value - 1)] = 1;
            printf("Priority item: %c = %d\n", input[i], value);
            total += value;
          }
        }
      }
    }
    free(matched);
  }
  free(input);

  fclose(fptr);

  printf("Total of priority items: %d\n", total); // 8515
  return 0;
}
