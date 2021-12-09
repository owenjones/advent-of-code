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

int letter_to_index(char letter) {
  // convert segment letters into an indexable value (a=0, b=1...)
  return letter - 97;
}

char index_to_letter(int index) {
  // convert indexes back into segment letters
  return index + 97;
}

int get_value(int segmentv) {
  // convert binary value of lit segments into display value
  switch(segmentv) {
    case 6: return 1;
    case 7: return 7;
    case 63: return 0;
    case 79: return 3;
    case 91: return 2;
    case 102: return 4;
    case 109: return 5;
    case 111: return 9;
    case 125: return 6;
    case 127: return 8;
  }
}

void sort(char inputs[10][8]) {

}

int solve_line(line* line) {
  // calculate how often we see each segment letter
  int frequencies[10];
  for(size_t i = 0; i < 10; i++) frequencies[i] = 0;
  for(size_t i = 0; i < 10; i++) {
    for(size_t j = 0; j < strlen(line->inputs[i]); j++) {
      frequencies[letter_to_index(line->inputs[i][j])]++;
    }
  }

  // map frequencies to segments {S0,S2,S4,S5} (the easy ones...)
  int values[10];
  for(size_t i = 0; i < 10; i++) {
    switch(frequencies[i]) {
      case 4: values[i] = 16;
      case 6: values[i] = 32;
      case 8: values[i] = 1;
      case 9: values[i] = 2;
      default: values[i] = 0;
    }
  }

  sort(line->inputs);

  // find S1 from 1 (input length 2)

  // find S6 from 4 (input length 4)

  // find S3 as the only remaining letter

  // work through each lit segment in each of the output values, and add
  // that segment's value to the total
  int total = 0;
  for(size_t i = 0; i < 4; i++) {
    for(size_t j = 0; j < strlen(line->outputs[i]); j++) {
      total += values[letter_to_index(line->outputs[i][j])];
    }
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
