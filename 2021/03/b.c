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

  char* inputs[l];
  for(size_t i = 0; i < l; i++) {
    inputs[i] = calloc(13, sizeof(char));
    fscanf(fptr, "%s", inputs[i]);
  }

  fclose(fptr);

  char* keep = calloc((l * 13), sizeof(char));
  memcpy(keep, inputs, sizeof(inputs));
  
  size_t m,n;
  int zeros = 0;
  int ones = 0;
  char keep;
  int pass = 0;
  
  for(size_t i = 0; i < 12; i++) {
    for(size_t j = 0; j < l; j++) {
      if(inputs[j][i] == '0') zeros++;
      if(inputs[j][i] == '1') ones++;
    }
    
    if(pass == 0) {
      if(zeros > ones) keep = '0';
      if(ones > zeros || ones == zeros) keep = '1';
      pass = 1;
    } else {
      if(zeros > ones) keep = '1';
      if(ones > zeros || ones == zeros) keep = '0';
    }
    
    for(size_t j = 0; j < l; j++) {
      m = 0;
      if(inputs[j][i] == keep) {
        
      }
      
      n = m;
    }
  }

  int o2 = 0;
  int co2 = 0;
  int life = 0;


  life = o2 * co2;
  printf("O2 Generator: %i\nCO2 Scrubber: %i\nLife Support Rating: %i\n", o2, co2, life);
  return 0;
}
