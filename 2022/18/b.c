#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct point {
  int x, y, z;
} point_t;

typedef struct list {
  int size;
  point_t** points;
} list_t;

void append(list_t* list, point_t* point) {
  list->points[list->size++] = point;
}

point_t* pop_head(list_t* list) {
  point_t* head = list->points[0];
  for(size_t i = 0; i < list->size; i++) {
    list->points[i] = list->points[(i + 1)];
  }
  list->size--;
  return head;
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int filled[100][100][100];
  int visited[100][100][100];
  size_t x, y, z;
  for(x = 0; x < 100; x++) {
    for(y = 0; y < 100; y++) {
      for(z = 0; z < 100; z++) {
        filled[x][y][z] = 0;
        visited[x][y][z] = 0;
      }
    }
  }
  
  int minx = 100, maxx = 0, miny = 100, maxy = 0, minz = 100, maxz = 0;
  int i, j, k;
  while(fscanf(fptr, "%d,%d,%d\n", &i, &j, &k) == 3) {
    i += 5; j += 5; k += 5; // to move points away from 0

    filled[i][j][k] = 1;
    
    minx = (i < minx) ? i : minx;
    maxx = (i > maxx) ? i : maxx;
    miny = (j < miny) ? j : miny;
    maxy = (j > maxy) ? j : maxy;
    minz = (k < minz) ? k : minz;
    maxz = (k > maxz) ? k : maxz;
  }
  fclose(fptr);
  
  minx -= 2;
  maxx += 2;
  miny -= 2;
  maxy += 2;
  minz -= 2;
  maxz += 2;
  
  list_t* queue = calloc(1, sizeof(list_t));
  queue->size = 0;
  queue->points = calloc((maxx * maxy * maxz), sizeof(point_t*));
  
  point_t* point = calloc(1, sizeof(point_t));
  point->x = minx;
  point->y = miny;
  point->z = minz;
  append(queue, point);

  int faces = 0;
  point_t *p = NULL, *n = NULL;
  while(queue->size > 0) {
    p = pop_head(queue);

    for(x = 0; x < 6; x++) {
      n = calloc(1, sizeof(point_t));
      n->x = p->x;
      n->y = p->y;
      n->z = p->z;
      switch(x) {
        case 0:
          n->x++;
          break;
          
        case 1:
          n->x--;
          break;
          
        case 2:
          n->y++;
          break;
          
        case 3:
          n->y--;
          break;
          
        case 4:
          n->z++;
          break;
          
        case 5:
          n->z--;
          break;
      }
      
      if(filled[n->x][n->y][n->z]) {
        faces++;
      } else if(!visited[n->x][n->y][n->z] 
        && n->x >= minx - 1
        && n->x <= maxx + 1
        && n->y >= miny - 1
        && n->y <= maxy + 1
        && n->z >= minz - 1
        && n->z <= maxz + 1) {
        visited[n->x][n->y][n->z] = 1;
        append(queue, n);
      }
    }
  }
    
  printf("Faces: %d\n", faces); // 2018
  return 0;
}
