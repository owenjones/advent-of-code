#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int btoi(int input[12]) {
  int out = 0;
  for(size_t i = 0, j = 11; i < 12; i++, j--) {
    out += input[i] * pow(2, j);
  }
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

  int o2s[1000][12], co2s[1000][12];
  memcpy(&o2s, &inputs, sizeof(int)*1000*12);
  memcpy(&co2s, &inputs, sizeof(int)*1000*12);

  int o2 = 0, co2 = 0;
  int o2i, co2i;
  int o2n = 1000, co2n = 1000;
  for(size_t b = 0; b < 12; b++) {
    o2i = 0, co2i = 0;

    // find most common bit in O2 list
    int n = 0, m = 0;
    for(size_t i = 0; i < o2n; i++) n += o2s[i][b];
    m = (2*n >= o2n);

    // filter O2 list to keep inputs with matching bit
    if(o2 == 0) {
      for(size_t i = 0; i < o2n; i++) {
        if(o2s[i][b] == m) {
          memcpy(&o2s[o2i], &o2s[i], sizeof(int)*12);
          o2i++;
        }
      }
    }

    // find most common bit in CO2 list
    n = 0, m = 0;
    for(size_t i = 0; i < co2n; i++) n += co2s[i][b];
    m = (2*n < co2n);

    // filter CO2 list to keep inputs without matching bit
    if(co2 == 0) {
      for(size_t i = 0; i < co2n; i++) {
        if(co2s[i][b] == m) {
          memcpy(&co2s[co2i], &co2s[i], sizeof(int)*12);
          co2i++;
        }
      }
    }

    if(o2i == 1 && o2 == 0) o2 = btoi(o2s[0]);
    if(co2i == 1 && co2 == 0) co2 = btoi(co2s[0]);

    memcpy(&o2n, &o2i, sizeof(int));
    memcpy(&co2n, &co2i, sizeof(int));
  }

  int life = o2 * co2; // 3368358
  printf("O2 Generator: %i\nCO2 Scrubber: %i\nLife Support Rating: %i\n", o2, co2, life);
  return 0;
}
