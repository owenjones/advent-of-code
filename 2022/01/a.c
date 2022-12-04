#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char file[] = "test_input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  size_t elf = 0;
  int* calories = calloc(250, sizeof(int));

  char* line = NULL;
  size_t len = 0;

  while(getline(&line, &len, fptr) != -1) {
    if(strcmp(line, "\n") == 0) {
      elf++;
    } else {
      calories[elf] += atoi(line);
    }
  }

  fclose(fptr);

  int max = 0;
  for(size_t i = 0; i < elf; i++) {
    max = (calories[i] > max) ? calories[i] : max;
  }

  free(calories);
  printf("Max calories carried: %d\n", max); // 71300
  return 0;
}
