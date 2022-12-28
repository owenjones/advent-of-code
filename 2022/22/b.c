#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

/*
Mapping of input to cube faces:
   [1][2]
   [3]
[4][5]
[6]
*/

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
  
  // Start in top-left of face 1
  i = 50;
  j = 0;
  
  uint8_t next, f = 0, nf; // 0 = E, 1 = S, 2 = W, 3 = N
  int steps, x, y;
  char dir;
  size_t p = 0, s;
  while(sscanf(&instructions[p], "%d%c", &steps, &dir) == 2) {
    // hacky way to progress through the string...
    p += (steps >= 100) ? 4 : ((steps >= 10) ? 3 : 2);
    
    for(s = 0; s < steps; s++) {
      switch(f) {
        case 0: // heading East
          next = grid[i + 1][j];
          if(next == 2) break;
          if(next == 0) {
            
            /*
            right from 2 (  0 <= y < 50 ) onto 5: facing W, x=99, y=-y+149
            right from 3 ( 50 <= y < 100) onto 2: facing N, x=y+50, y=49
            right from 5 (100 <= y < 150) onto 2: facing W, x=149 , y=-y+149
            right from 6 (150 <= y < 200) onto 5: facing N, x=y-100 , y=149
            */
            
            if(j >= 0 && j < 50) {
              x = 99;
              y = -j + 149;
              nf = 2;
            } else if(j >= 50 && j < 100) {
              x = j + 50;
              y = 49;
              nf = 3;
            } else if(j >= 100 && j < 150) {
              x = 149;
              y = -j + 149;
              nf = 2;
            } else if(j >= 150 && j < 200) {
              x = j - 100;
              y = 149;
              nf = 3;
            }
            
            if(grid[x][y] != 2) {
              i = x;
              j = y;
              f = nf;
            } else {
              next = 2;
            }
          } else if(next == 1) i++;
          break;
          
        case 1: // heading South
          next = grid[i][j + 1];
          if(next == 2) break;
          if(next == 0) {
            
            /*
            down from 6 (  0 <= x < 50 ) onto 2: facing S, x=x+100, y=0
            down from 5 ( 50 <= x < 100) onto 6: facing W, x=49, y=x+100
            down from 2 (100 <= x < 150) onto 3: facing W, x=99, y=x-50
            */
            
            if(i >= 0 && i < 50) {
              x = i + 100;
              y = 0;
              nf = 1;
            } else if(i >= 50 && i < 100) {
              x = 49;
              y = i + 100;
              nf = 2;
            } else if(i >= 100 && i < 150) {
              x = 99;
              y = i - 50;
              nf = 2;
            }
            
            if(grid[x][y] != 2) {
              i = x;
              j = y;
              f = nf;
            } else {
              next = 2;
            }
          } else if(next == 1) j++;
          break;
          
        case 2: // heading West
          next = (i - 1 < 0) ? 0 : grid[i - 1][j];
          if(next == 2) break;
          if(next == 0) {
            
            /*
            left from 1 (  0 <= y < 50 ) onto 4: facing E, x=0, y=-y+149
            left from 3 ( 50 <= y < 100) onto 4: facing S, x=y-50 , y=100
            left from 4 (100 <= y < 150) onto 1: facing E, x=50, y=-y+149
            left from 6 (150 <= y < 200) onto 1: facing S, x=y-100, y=0
            */
            
            if(j >= 0 && j < 50) {
              x = 0;
              y = -j + 149;
              nf = 0;
            } else if(j >= 50 && j < 100) {
              x = j - 50;
              y = 100;
              nf = 1;
            } else if(j >= 100 && j < 150) {
              x = 50;
              y = -j + 149;
              nf = 0;
            } else if(j >= 150 && j < 200) {
              x = j - 100;
              y = 0;
              nf = 1;
            }
            
            if(grid[x][y] != 2) {
              i = x;
              j = y;
              f = nf;
            } else {
              next = 2;
            }
          } else if(next == 1) i--;
          break;
          
        case 3: // heading North
          next = (j - 1 < 0) ? 0 : grid[i][j - 1];
          if(next == 0) {
            
            /*
            up from 4 (  0 <= x < 50 ) onto 3: facing E, x=50, y=x+50
            up from 1 ( 50 <= x < 100) onto 6: facing E, x=0, y=x+100
            up from 2 (100 <= x < 150) onto 6: facing N, x=x-100, y=199
            */
            
            if(i >= 0 && i < 50) {
              x = 50;
              y = i + 50;
              nf = 0;
            } else if(i >= 50 && i < 100) {
              x = 0;
              y = i + 100;
              nf = 0;
            } else if(i >= 100 && i < 150) {
              x = i - 100;
              y = 199;
              nf = 3;
            }
            
            if(grid[x][y] != 2) {
              i = x;
              j = y;
              f = nf;
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
  printf("Final password using cube magic = %d\n", final); // 153203

  return 0;
}
