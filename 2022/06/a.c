#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int find_start(char* input, int len) {
  for(int i = 0; i < len; i++) {
    if((input[i] != input[i+1]) && (input[i] != input[i+2])
    && (input[i] != input[i+3]) && (input[i+1] != input[i+2])
    && (input[i+1] != input[i+3]) && (input[i+2] != input[i+3])) {
      return i + 4;
    }
  }
  return -1;
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  char input[4096];
  fscanf(fptr, "%s", input);
  int len = strlen(input);
  int marker = find_start(input, len);

  fclose(fptr);

  printf("First marker after character: %d\n", marker); // 1361
  return 0;
}
