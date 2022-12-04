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
  int i;
  int lines = 0;
  
  while(fscanf(fptr, "%d-%d,%d-%d", &a, &b, &c, &d) > 0) {
    upper = calloc(100, sizeof(int));
    lower = calloc(100, sizeof(int));
    
    for(i = a; i <= b; i++) {
      upper[i]++;
    }
    
    for(i = c; i <= d; i++) {
      lower[i]++;
    }
    
    for(i = 0; i < 100; i++) {
      if(upper[i] == 1 && lower[i] == 1) {
        lines++;
        break;
      }
    }
    
    free(upper);
    free(lower);
  }

  fclose(fptr);

  printf("Total lines: %d\n", lines); // 849
  return 0;
}
