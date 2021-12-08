#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char inputs[10][8];
  int matched[10];
  char outputs[4][8];
} line;

line* parse_line(char inputs[15][8]) {
  line* line = calloc(1, sizeof(line));
  memcpy(line->inputs, inputs, sizeof(char)*10);
  memcpy(line->outputs, &inputs[11], sizeof(char)*4);
  return line;
}

int solve_line(line* line) {
  int total = 0;
  for(size_t i = 0; i < 10; i++) {
    // solve input
  }

  for(size_t i = 0; i < 4; i++) {
    // map each output to matches and sum value
  }
  return total;
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int total = 0;
  line* line;
  char inputs[15][8];
  for(size_t i = 0; i < 200; i++) {
    for(size_t j = 0; j < 15; j++) {
      fscanf(fptr, "%s", inputs[j]);
    }

    line = parse_line(inputs);
    total += solve_line(line);
  }

  fclose(fptr);

  printf("Total: %i\n", total); // answer:
  return 0;
}
