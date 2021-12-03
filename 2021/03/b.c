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

  for(size_t i = 0; i < l; i++) {
    fscanf(fptr, "%s", input);

  }

  fclose(fptr);


  int o2, co2, life;


  life = o2 * co2;
  printf("\nO2 Generator: %i\nCO2 Scrubber: %i\nLife Support Rating: %i\n", o2, co2, life);
  return 0;
}
