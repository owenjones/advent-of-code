#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int btoi(int input[12]) {
  int out = 0;
  for(size_t i = 0, j = 11; i < 12; i++, j--) {
    printf("%i", input[i]);
    out += input[i] * pow(2, j);
  }
  printf("\n");
  return out;
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  // parse inputs
  int inputs[1000][12];
  for(size_t i = 0; i < 1000; i++) {
    for(size_t j = 0; j < 12; j++) {
      fscanf(fptr, "%1i", &inputs[i][j]);
    }
  }

  fclose(fptr);

  

  int life = o2 * co2;
  printf("\nO2 Generator: %i\nCO2 Scrubber: %i\nLife Support Rating: %i\n", o2, co2, life);
  return 0;
}
