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

  char* input = NULL;
  size_t bufsize = 0;
  if(getline(&input, &bufsize, fptr) == -1) {
    printf("Error reading input line\n");
    exit(1);
  }
  fclose(fptr);

  int instructions[15000];
  for(size_t i = 0; i < strlen(input); i++) {
    char c = input[i];
    if(c == 10) continue; // skip new lines
    instructions[i] = (c == 60) ? 0 : 1; // < = 0, > = 1
  }
  free(input);

  printf("");
  return 0;
}
