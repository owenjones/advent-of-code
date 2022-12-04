#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int value(char input) {
  switch(input) {
    case 'A': // rock
      return 1;
    case 'B': // paper
      return 2;
    case 'C': // scissors
      return 3;
    case 'X': // rock
      return 1;
    case 'Y': // paper
      return 2;
    case 'Z': // scissors
      return 3;
  }

  return 0;
}

int outcome(char a, char b) {
  if(value(a) == value(b)) return 3;
  if((b == 'Y' && a == 'A') || (b == 'Z' && a == 'B') || (b == 'X' && a == 'C')) return 6;
  return 0;
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  char input;
  char response;
  int score = 0;
  int total = 0;

  while(fscanf(fptr, "%c %c\n", &input, &response) > 0) {
    score = value(response) + outcome(input, response);
    total += score;
  }

  fclose(fptr);

  printf("Total score: %d\n", total); // 11906
  return 0;
}
