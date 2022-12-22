#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  size_t i, j;
  uint8_t grid[250][250];
  for(i = 0; i < 250; i++) {
    for(j = 0; j < 250; j++) {
      grid[i][j] = 0;
    }
  }
  
  char instructions[10000];

  char* input = NULL;
  size_t bufsize = 0;
  
  j = 0;
  while(getline(&input, &bufsize, fptr) != -1) {
    if(strlen(input) > 1) {
      for(i = 0; i < (strlen(input) - 1); i++) {
        switch(input[i]) {
          case ' ':
            grid[i][j] = 0;
            break;
            
          case '.':
            grid[i][j] = 1;
            break;
            
          case '#':
            grid[i][j] = 2;
            break;
        }
      }
      j++;
    } else if(strlen(input) == 1) {
      getline(&input, &bufsize, fptr);
      strcpy(instructions, input);
    }
  }
  fclose(fptr);
  free(input);

  j = 0;
  for(i = 0; i < 150; i++) if(grid[i][j] == 1) break;
  int8_t f = 0; // 0 = E, 1 = S, 2 = W, 3 = N
  uint8_t next;
  int steps;
  char dir;
  size_t p = 0, x, y;
  while(sscanf(&instructions[p], "%d%c", &steps, &dir) == 2) {
    // hacky way to progress through the string...
    p += (steps >= 100) ? 4 : ((steps >= 10) ? 3 : 2);
    
    for(x = 0; x < steps; x++) {
      switch(f) {
        case 0: 
          next = grid[i + 1][j]; 
          if(next == 2) break;
          if(next == 0) {
            y = 0;
            while(grid[y][j] == 0) y++;
            if(grid[y][j] != 2) {
              i = y;
            } else {
              next = 2;
            }
          } else if(next == 1) i++;
          break;
          
        case 1: 
          next = grid[i][j + 1];
          if(next == 2) break;
          if(next == 0) {
            y = 0;
            while(grid[i][y] == 0) y++;
            if(grid[i][y] != 2) {
              j = y;
            } else {
              next = 2;
            }
          } else if(next == 1) j++;
          break;
          
        case 2:
          next = grid[i - 1][j];
          if(next == 2) break;
          if(next == 0 || (i - 1) < 0) {
            y = 249;
            while(grid[y][j] == 0) y--;
            if(grid[y][j] != 2) {
              i = y;
            } else {
              next = 2;
            }
          } else if(next == 1) i--;
          break;
          
        case 3:
          next = grid[i][j - 1];
          if(next == 0 || (j - 1) < 0) {
            y = 249;
            while(grid[i][y] == 0) y--;
            if(grid[i][y] != 2) {
              j = y;
            } else {
              next = 2;
            }
          } else if(next == 1) j--;
          break;
      }
      
      if(next == 2) break;
    }
    
    if(dir == 'L') f = (4 + (f - 1)) % 4;
    if(dir == 'R') f = (f + 1) % 4;
  }

  int final = (1000 * (j + 1)) + (4 * (i + 1)) + f;
  printf("Final password = %d\n", final); // 149138

  return 0;
}
