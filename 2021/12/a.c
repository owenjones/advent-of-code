#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum size{Big, Small};

typedef struct cave {
  char id[6];
  enum size size;
} cave_t;

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  char in[6], out[6];
  while(fscanf(fptr, "%s-%s", &in, &out) > 0) {

  }

  fclose(fptr);

  int paths = 0;
  printf("Paths through the system: %i\n", paths);
  return 0;
}
