#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int score(char input) {
  switch(input) {
    case '(': return 1;
    case '[': return 2;
    case '{': return 3;
    case '<': return 4;
    default: return 0;
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

uint64_t test_line(char input[100]) {
  char symbols[100];
  int index = 0;
  
  for(size_t i = 0; i < strlen(input); i++) {
    if(is_open(input[i])) {
      symbols[index++] = input[i];
    }
    else if(is_close(input[i])) {
      if(type(input[i]) == type(symbols[index-1])) {
        index--;
      } else {
        return 0; // line is corrupt
      }
    }
  }
  
  uint64_t s = 0;
  for(size_t i = index ; i > 0; i--) {
    s *= 5;
    s += score(symbols[i-1]);
  }
  return s;
}

void sort(uint64_t* inputs, int len) {
  // sort smallest first
  int i = 0;
  uint64_t temp;
  while(i < len - 1) {
    if(inputs[i+1] < inputs[i]) {
      temp = inputs[i];
      inputs[i] = inputs[i+1];
      inputs[i+1] = temp;
      i = 0;
    } else {
      i++;
    }
  }
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  uint64_t score, scores[98];
  int ind = 0;
  char line[100];
  for(size_t i = 0; i < 98; i++) {
    fscanf(fptr, "%s", line);
    score = test_line(line);
    if(score > 0) scores[ind++] = score;
  }
  
  fclose(fptr);
  
  sort(scores, ind);
  score = scores[(ind / 2)];
  printf("Middle score (of %i scores): %llu\n", ind, score); // 2360030859
  return 0;
}
