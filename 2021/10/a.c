#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int score(char input) {
  switch(input) {
    case ')': return 3;
    case ']': return 57;
    case '}': return 1197;
    case '>': return 25137;
    default: return -1;
  }
}

int type(char input) {
  switch(input) {
    case '(':
    case ')':
      return 1;
    case '[':
    case ']':
      return 2;
    case '{':
    case '}':
      return 3;
    case '<':
    case '>':
      return 4;
    default:
      return -1;
  }
}

int is_open(char input) {
  switch(input) {
    case '(':
    case '[':
    case '{':
    case '<':
      return 1;
    default:
      return 0;
  }
}

int is_close(char input) {
  switch(input) {
    case ')':
    case ']':
    case '}':
    case '>':
      return 1;
    default:
      return 0;
  }
}

int test_line(char input[100]) {
  char symbols[100];
  int index = 0;
  
  for(size_t i = 0; i < strlen(input); i++) {
    if(is_open(input[i])) {
      symbols[index++] = input[i];
    }
    else if(is_close(input[i])) {
      if(type(input[i]) == type(symbols[index-1])) {
        symbols[index--] = 0;
      } else {
        return score(input[i]);
      }
    }
  }
  return 0;
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

  printf("Total score: %i\n", score); // 321237
  return 0;
}
