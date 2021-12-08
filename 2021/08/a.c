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

  char input[8];
  int count;
  for(size_t i = 0; i < 200; i++) {
    for(size_t j = 0; j < 15; j++) {
      fscanf(fptr, "%s", input);
      if(j > 10) {
        int len = strlen(input);
        switch(len) {
          case 2:
          case 3:
          case 4:
          case 7:
            count++;
            break;
        }
      }
    }
  }
  fclose(fptr);

  printf("Matched 1, 4, 7 and 8 %i times\n", count); // answer: 349
  return 0;
}
