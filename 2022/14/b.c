#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define X 800
#define Y 800

int min(int a, int b) {
  return (a < b) ? a : b;
}

int max(int a, int b) {
  return (b > a) ? b : a;
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  uint8_t grid[X][Y];
  
  size_t i, j;
  for(j = 0; j < Y; j++) {
    for(i = 0; i < X; i++) {
      grid[i][j] = 0;
    }
  }
  
  int p, x, minx, maxx, y, miny, maxy, f = 0;
  int points[50][2];
  char pair[50][10];
  char *string = NULL;

  char *input = NULL;
  size_t bufsize = 0;
  while(getline(&input, &bufsize, fptr) != -1) {
    // extract each x,y pair
    p = 0;
    string = strtok(input, " -> ");
    while(string != NULL) {
      strcpy(pair[p++], string);
      string = strtok(NULL, " -> ");
    }
    
    // extract individual coordinates into points
    for(i = 0; i < p; i++) {
      string = strtok(pair[i], ",");
      j = 0;
      while(string != NULL) {
        points[i][j] = atoi(string);
        string = strtok(NULL, ",");
        j++;
      }
    }
    
    // mark line between each point
    for(i = 0; i < (p - 1); i++) {
      minx = min(points[i][0], points[(i + 1)][0]);
      maxx = max(points[i][0], points[(i + 1)][0]);
      miny = min(points[i][1], points[(i + 1)][1]);
      maxy = max(points[i][1], points[(i + 1)][1]);
      
      for(y = miny; y <= maxy; y++) {
        for(x = minx; x <= maxx; x++) {
          grid[x][y] = 1;
        }
      }
      
      f = (f > maxy) ? f : maxy;
    }
  }
  fclose(fptr);
  free(input);
  free(string);
  
  f += 2;
  for(i = 0; i < X; i++) {
    grid[i][f] = 1;
  }

  int units = 0;
  while(1) {
    x = 500;
    y = 0;
    
    while(1) {
      if(grid[x][(y + 1)] == 0) {
        // down
        y++;
        continue;
      } else if(grid[(x - 1)][(y + 1)] == 0) {
        // down-left
        x--;
        y++;
        continue;
      } else if(grid[(x + 1)][(y + 1)] == 0) {
        // down-right
        x++;
        y++;
        continue;
      }
      
      units++;
      grid[x][y] = 2;
      break;
    }
    
    if(x == 500 && y == 0) break;
  }

  printf("Units of sand to fill structure: %d\n", units); // 24377
  return 0;
}
