#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
Segment definition:

 — S0 —
|      |
S5    S1
|      |
 — S6 —
|      |
S4    S2
|      |
 — S3 —
 
Binary representation is little endian.
*/

typedef struct {
  char inputs[10][8];
  char outputs[4][8];
} line;

line* parse_line(char inputs[15][8]) {
  line* line = calloc(1, sizeof(line));
  memcpy(line->inputs, inputs, sizeof(char)*10*8);
  memcpy(line->outputs, &inputs[11], sizeof(char)*4*8);
  return line;
}

int letter_to_index(char letter) {
  // convert segment letters into index value (a=0, b=1...)
  return letter - 97;
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
    default: return -1;
  }
}

int solve_line(line* line) {
  // calculate frequency of each segment letter
  int frequencies[7];
  for(size_t i = 0; i < 7; i++) frequencies[i] = 0;
  for(size_t i = 0; i < 10; i++) {
    for(size_t j = 0; j < strlen(line->inputs[i]); j++) {
      frequencies[letter_to_index(line->inputs[i][j])]++;
    }
  }

  // map frequencies to segments {S2,S4,S5} (the easy ones...)
  int values[7];
  for(size_t i = 0; i < 7; i++) {
    switch(frequencies[i]) {
      case 4:
        values[i] = 16;
        break;
      case 6:
        values[i] = 32;
        break;
      case 9:
        values[i] = 4;
        break;
      default:
        values[i] = 0;
        break;
    }
  }

  // find S2 from 1 (input length 2)
  for(size_t i = 0; i < 10; i++) {
    if(strlen(line->inputs[i]) == 2) {
      char x = line->inputs[i][0];
      char y = line->inputs[i][1];
      if(values[letter_to_index(x)] == 0) {
        values[letter_to_index(x)] = 2;
      } else {
        values[letter_to_index(y)] = 2;
      }
      break;
    }
  }

  // find S0 from 7 (input length 3)
  for(size_t i = 0; i < 10; i++) {
    if(strlen(line->inputs[i]) == 3) {
      for(size_t j = 0; j < 3; j++) {
        if(values[letter_to_index(line->inputs[i][j])] == 0) {
          values[letter_to_index(line->inputs[i][j])] = 1;
        }
      }
      break;
    }
  }

  // find S6 from 4 (input length 4)
  for(size_t i = 0; i < 10; i++) {
    if(strlen(line->inputs[i]) == 4) {
      for(size_t j = 0; j < 4; j++) {
        if(values[letter_to_index(line->inputs[i][j])] == 0) {
          values[letter_to_index(line->inputs[i][j])] = 64;
        }
      }
      break;
    }
  }

  // find S3 as the only remaining letter
  for(size_t i = 0; i < 7; i++) 
  values[i] = (values[i] == 0) ? 8 : values[i];

  // work through each lit segment in each of the output strings, add
  // that segment's value to the output total, then convert to get digit
  // multiply these by powers of 10 to get final output value, and sum all together
  int total = 0, output, segv;
  for(size_t i = 0; i < 4; i++) {
    output = 0;
    for(size_t j = 0; j < strlen(line->outputs[i]); j++) {
      output += values[letter_to_index(line->outputs[i][j])];
    }
    
    total += pow(10, (3 - i)) * get_value(output);
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

  printf("Total: %i\n", total); // answer: 1070957
  return 0;
}
