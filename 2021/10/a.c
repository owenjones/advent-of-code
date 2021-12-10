#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int score(char input) {
  switch(input) {
    case '(': return 3;
    case '[': return 57;
    case '{': return 1197;
    case '<': return 25137;
    default: return -1;
  }
}

int test_line(char input[100]) {
  for(size_t i = 0; i < strlen(input); i++) {

  }
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int score = 0;
  char line[100];
  for(size_t i = 0; i < 98; i++) {
    fscanf(fptr, "%s", line);
    score += test_line(line);
  }

  fclose(fptr);

  printf("Total score: %i\n", score);
  return 0;
}
