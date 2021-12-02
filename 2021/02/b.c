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

  int x, y, aim = 0;

  char* command = calloc(8, sizeof(char));
  int amount;

  for(size_t i = 0; i < 1000; i++) {
    fscanf(fptr, "%s %i", command, &amount);

    if(strcmp(command, "forward") == 0) {
      x += amount;
      y += amount * aim;
    }

    if(strcmp(command, "backward") == 0) {
      x -= amount;
    }

    if(strcmp(command, "down") == 0) {
      aim += amount;
    }

    if(strcmp(command, "up") == 0) {
      aim -= amount;
    }
  }

  fclose(fptr);

  printf("Distance: %i\nDepth: %i\nTotal: %i\n", x, y, (x*y));
  return 0;
}
