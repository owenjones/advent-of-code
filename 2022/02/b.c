#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int response(char input, char outcome) {
  switch(outcome) {
    case 'X':
      switch(input) {
        case 'A':
          return 3;
        case 'B':
          return 1;
        case 'C':
          return 2;
      }

    case 'Y':
      switch(input) {
        case 'A':
          return 1;
        case 'B':
          return 2;
        case 'C':
          return 3;
      }

    case 'Z':
      switch(input) {
        case 'A':
          return 2;
        case 'B':
          return 3;
        case 'C':
          return 1;
      }
  }
}

int outcome(char input) {
  switch(input) {
    case 'X': // lose
      return 0;
    case 'Y': // draw
      return 3;
    case 'Z': // win
      return 6;
  }
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  char in;
  char out;
  int score = 0;
  int total = 0;

  while(fscanf(fptr, "%c %c\n", &in, &out) > 0) {
    score = response(in, out) + outcome(out);
    total += score;
  }

  fclose(fptr);

  printf("Total score: %d\n", total);
  return 0;
}
