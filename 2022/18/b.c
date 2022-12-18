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

  uint8_t filled[100][100][100];
  size_t x, y, z;
  for(x = 0; x < 100; x++) {
    for(y = 0; y < 100; y++) {
      for(z = 0; z < 100; z++) {
        filled[x][y][z] = 0;
      }
    }
  }

  int minx = 100, maxx = 0, miny = 100, maxy = 0, minz = 100, maxz = 0;
  int i, j, k, c = 0, cubes[10000][3];
  while(fscanf(fptr, "%d,%d,%d\n", &i, &j, &k) == 3) {
    cubes[c][0] = i;
    cubes[c][1] = j;
    cubes[c][2] = k;
    c++;
    
    filled[i][j][k] = 1;
    
    minx = (i < minx) ? i : minx;
    maxx = (i > maxx) ? i : maxx;
    miny = (j < miny) ? j : miny;
    maxy = (j > maxy) ? j : maxy;
    minz = (k < minz) ? k : minz;
    maxz = (k > maxz) ? k : maxz;
  }
  fclose(fptr);
  
  for(z = 0; z < 100; z++) {
    for(y = 0; y < 100; y++) {
      x = 0;
      while(x <= 99) {
        printf("Filling (%zu, %zu, %zu)\n", x, y, z);
        filled[x][y][z] = 1;
        if((x + 1) < 100 && filled[(x + 1)][y][z] == 0) {
          x++;
          continue;
        }
        break;
      }
      
      x = 99;
      while(x >= 0) {
        printf("Filling (%zu, %zu, %zu)\n", x, y, z);
        filled[x][y][z] = 1;
        if((x - 1) >= 0 && filled[(x - 1)][y][z] == 0) {
          x--;
          continue;
        }
        break;
      }
    }
    
    for(x = 0; x < 100; x++) {
      y = 0;
      while(y <= 99) {
        printf("Filling (%zu, %zu, %zu)\n", x, y, z);
        filled[x][y][z] = 1;
        if((y + 1) < 100 && filled[x][(y + 1)][z] == 0) {
          y++;
          continue;
        }
        break;
      }
      
      y = 99;
      while(y >= 0) {
        printf("Filling (%zu, %zu, %zu)\n", x, y, z);
        filled[x][y][z] = 1;
        if((y - 1) >= 0 && filled[x][(y - 1)][z] == 0) {
          y--;
          continue;
        }
        break;
      }
    }
  }
  
  for(z = 99; z >= 0; z--) { 
    if(z > 99) break;   
    for(y = 0; y < 100; y++) {
      x = 0;
      while(x <= 99) {
        printf("Filling (%zu, %zu, %zu)\n", x, y, z);
        filled[x][y][z] = 1;
        if((x + 1) < 100 && filled[(x + 1)][y][z] == 0) {
          x++;
          continue;
        }
        break;
      }
      
      x = 99;
      while(x >= 0) {
        printf("Filling (%zu, %zu, %zu)\n", x, y, z);
        filled[x][y][z] = 1;
        if((x - 1) >= 0 && filled[(x - 1)][y][z] == 0) {
          x--;
          continue;
        }
        break;
      }
    }
    
    for(x = 0; x < 100; x++) {
      y = 0;
      while(y <= 99) {
        printf("Filling (%zu, %zu, %zu)\n", x, y, z);
        filled[x][y][z] = 1;
        if((y + 1) < 100 && filled[x][(y + 1)][z] == 0) {
          y++;
          continue;
        }
        break;
      }
      
      y = 99;
      while(y >= 0) {
        printf("Filling (%zu, %zu, %zu)\n", x, y, z);
        filled[x][y][z] = 1;
        if((y - 1) >= 0 && filled[x][(y - 1)][z] == 0) {
          y--;
          continue;
        }
        break;
      }
    }
  }
  
  // smashing part 2 back into part 1...
  for(z = 0; z < 100; z++) {
    for(y = 0; y < 100; y++) {
      for(x = 0; x < 100; x++) {
        if(filled[x][y][z] == 0) {
          printf("Pocket at (%zu, %zu, %zu)\n", x, y, z);
          // filled[x][y][z] = 1;
          
          cubes[c][0] = x;
          cubes[c][1] = y;
          cubes[c][2] = z;
          c++;
        }
      }
    }
  }
  
  int adjacent = 0, l, m, n;
  for(x = 0; x < c; x++) {
    for(y = 0; y < c; y++) {
      if(x == y) continue;

      l = (abs(cubes[x][0] - cubes[y][0]) == 1) && (cubes[x][1] == cubes[y][1]) && (cubes[x][2] == cubes[y][2]);

      m = (abs(cubes[x][1] - cubes[y][1]) == 1) && (cubes[x][0] == cubes[y][0]) && (cubes[x][2] == cubes[y][2]);

      n = (abs(cubes[x][2] - cubes[y][2]) == 1) && (cubes[x][1] == cubes[y][1]) && (cubes[x][0] == cubes[y][0]);

      if(l ^ m ^ n) {
        adjacent++;
      }
    }
  }

  int total = (6 * c) - adjacent;
  printf("Total surface area: %d\n", total);

  // 1570 - too low
  // 1876 - too low
  // 1984 - too low
  // 2794 - X
  // 2325 - X
  // 2865 - X
  // 3220 - too high

  return 0;
}
