#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

char snafcode(int i) {
  switch(i) {
    case  2: return '2';
    case  1: return '1';
    case  0: return '0';
    case -1: return '-';
    case -2: return '=';
  }
  return '?';
}

int64_t snafu2i(char* snafu) {
  int64_t output = 0;
  int decode, base;
  char c;
  for(int i = (strlen(snafu) - 1); i >= 0; i--) {
    base = strlen(snafu) - 1 - i;
    c = snafu[i];
    
    if(c == '=') decode = -2;
    else if(c == '-') decode = -1;
    else decode = (c - 48);
    
    output += decode * pow(5, base);
  }
  
  return output;
}

char* i2snafu(int64_t input) {
  char* output = calloc(100, sizeof(char));
  
  size_t n = 0;
  while(input > 0) {
    int64_t temp = input % 5;
    if(temp >= 3) temp -= 5;
    output[n++] = snafcode(temp);
    input -= temp;
    input /= 5;
  }
  
  char* reversed = calloc(100, sizeof(char));
  size_t i;
  for(i = 0; i < n; i++) reversed[i] = output[(n - 1 - i)];
  free(output);
  
  reversed[i++] = '\0';
  return reversed;
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int64_t total = 0;
  char input[50];
  while(fscanf(fptr, "%s", input) == 1) {
    total += snafu2i(input);
  }
  fclose(fptr);

  printf("Decimal output to test: %lld\n", total);
  char* output = i2snafu(total);
  printf("SNAFU to supply to the console: %s\n", output); // 2=--00--0220-0-21==1
  free(output);
  return 0;
}
