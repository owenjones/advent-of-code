#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int btoi(int input[12]) {

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

  // find most/least common bits
  float sum[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int most[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  for(size_t i = 0; i < 1000; i++) {
    for(size_t j = 0; j < 12; j++) sum[j] += inputs[i][j];
  }
  printf("Most common bits: ");
  for(size_t i = 0; i < 12; i++) {
    sum[i] /= 1000;
    if(sum[i] >= 0.5) most[i] = 1;
    printf("%i", most[i]);
  }
  printf("\n\n");

  // run through inputs and compare
  int o2s[1000][12], co2s[1000][12];
  memcpy(o2s, inputs, sizeof(int[1000][12]));
  memcpy(co2s, inputs, sizeof(int[1000][12]));

  int o2 = 0, o2n = 1000, o2i, co2 = 0, co2n = 1000, co2i;
  for(size_t i = 0; i < 12; i++) {
    memcpy(&o2i, &o2n, sizeof(int));
    memcpy(&co2i, &co2n, sizeof(int));

    // run through O2 list
    for(size_t j = 0; j < o2i; j++) {
      o2n = 0;
      if(o2s[j][i] == most[j]) {
        memcpy(o2s[o2n], o2s[i], sizeof(int[12]));
        o2n++;
      }
    }

    // run through CO2 list
    for(size_t j = 0; j < co2i; j++) {
      co2n = 0;
      if(co2s[j][i] == most[j]) {
        memcpy(co2s[o2n], co2s[i], sizeof(int[12]));
        co2n++;
      }
    }

    printf("bit pos %i, o2n=%i, co2n=%i\n", i, o2n, co2n);

    if(o2n == 1) o2 = 1;
    if(co2n == 1) co2 = 1;
  }

  int life = o2 * co2;
  printf("O2 Generator: %i\nCO2 Scrubber: %i\nLife Support Rating: %i\n", o2, co2, life);
  return 0;
}
