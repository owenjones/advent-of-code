#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  size_t elf = 0;
  int* calories = calloc(512, sizeof(int));

  char* line = NULL;
  size_t len = 0;

  while(getline(&line, &len, fptr) != -1) {
    if(strcmp(line, "\n") == 0) {
      elf++;
    } else {
      calories[elf] += atoi(line);
    }
  }
  free(line);
  fclose(fptr);

  int temp = 0;
  for(size_t i = 0; i < elf; i++) {
    for(size_t j = 0; j < (elf - i); j++) {
      if(calories[j] < calories[j+1]) {
        temp = calories[j];
        calories[j] = calories[j+1];
        calories[j+1] = temp;
      }
    }
  }

  int total = calories[0] + calories[1] + calories[2];
  free(calories);

  printf("Calories carried by top three: %d\n", total); // 209691
  return 0;
}
