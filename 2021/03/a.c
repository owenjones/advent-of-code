#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char file[] = "input.txt";
  int l = 1000;
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  char* input = calloc(13, sizeof(char));
  int* zeros = calloc(12, sizeof(int));
  int* ones = calloc(12, sizeof(int));

  for(size_t i = 0; i < l; i++) {
    fscanf(fptr, "%s", input);

    for(size_t j = 0; j < strlen(input); j++) {
      if(input[j] == '0') {
        zeros[j]++;
      }

      if(input[j] == '1') {
        ones[j]++;
      }
    }
  }

  fclose(fptr);

  char* output = calloc(13, sizeof(char));
  for(size_t i = 0; i < 12; i++) {
    printf("%i: %i/%i\n", i, zeros[i], ones[i]);
    output[i] = (zeros[i] > ones[i]) ? '0' : '1';
  }

  printf("Output: %s\n", output);

  u_int32_t power;
  u_int16_t gamma, epsilon = 0;

  gamma = strtol(output, NULL, 2);
  epsilon = ~gamma - 61440; // ignore 4 MSB of gamma (not set by inputs)
  power = gamma * epsilon;

  printf("\nGamma: %i\nEpsilon: %i\nPower: %i\n", gamma, epsilon, power);
  return 0;
}
