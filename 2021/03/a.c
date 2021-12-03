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
  int zeros[12];
  int ones[12];

  for(size_t i = 0; i < l; i++) {
    fscanf(fptr, "%s", input);

    printf("Test: %s (%i)\n", input, strlen(input));
    for(size_t j = 0; j < strlen(input); j++) {
      if(strcmp(&input[j], "0") == 0) {
        zeros[j]++;
      }

      if(strcmp(&input[j], "1") == 0) {
        ones[j]++;
      }
    }
  }

  fclose(fptr);

  char* output = calloc(13, sizeof(char));
  for(size_t i = 0; i < 13; i++) {
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
