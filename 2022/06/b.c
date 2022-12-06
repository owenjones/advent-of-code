#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int inner_search(char* input, int n) {
  for(int i = 0; i < 14; i++) {
    for(int j = 0; j < 14; j++) {
      if(i == j) continue;
      if(input[n+i] == input[n+j]) return 0;
    }
  }

  return n + 14;
}

int find_start(char* input, int len) {
  int n = 0;
  int marker = 0;
  while(n < len) {
    marker = inner_search(input, n);
    if(marker) return marker;
    n++;
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

  printf("First marker after character: %d\n", marker); // 3263
  return 0;
}
