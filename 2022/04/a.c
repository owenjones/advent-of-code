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

  int a, b, c, d;
  int* upper = NULL;
  int* lower = NULL;
  int* overlap = NULL;
  int i;
  int lines = 0;
  
  while(fscanf(fptr, "%d-%d,%d-%d", &a, &b, &c, &d) > 0) {
    upper = calloc(100, sizeof(int));
    lower = calloc(100, sizeof(int));
    overlap = calloc(100, sizeof(int));
    
    for(i = a; i <= b; i++) {
      upper[i]++;
    }
    
    for(i = c; i <= d; i++) {
      lower[i]++;
    }
    
    int x = 1, y = 1;
    for(i = 0; i < 100; i++) {
      overlap[i] = upper[i] | lower[i];
      
      if(upper[i] != overlap[i]) x = 0;
      if(lower[i] != overlap[i]) y = 0;
    }
    
    if(x || y) lines++;
  }

  fclose(fptr);

  printf("Total lines: %d\n", lines);
  return 0;
}
