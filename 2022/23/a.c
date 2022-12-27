#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>

typedef struct elf {
  uint32_t now[2];
  uint32_t next[2];
} elf_t;

typedef struct elves {
  uint32_t count;
  elf_t** elf;
} elves_t;

void clear_grid(uint8_t grid[2000][2000]) {
  for(size_t i = 0; i < 2000; i++) {
    for(size_t j = 0; j < 2000; j++) {
      grid[i][j] = 0;
    }
  }
}

uint32_t count_empty(elves_t* elves) {
  uint32_t min_x = INT_MAX, min_y = INT_MAX, max_x = 0, max_y = 0;
  
  for(size_t e = 0; e < elves->count; e++) {
    min_x = (elves->elf[e]->now[0] < min_x) ? elves->elf[e]->now[0] : min_x;
    min_y = (elves->elf[e]->now[1] < min_y) ? elves->elf[e]->now[1] : min_y;
    max_x = (elves->elf[e]->now[0] > max_x) ? elves->elf[e]->now[0] : max_x;
    max_y = (elves->elf[e]->now[1] > max_y) ? elves->elf[e]->now[1] : max_y;
  }
  
  uint32_t total = (max_x - min_x + 1) * (max_y - min_y + 1);

  for(size_t e = 0; e < elves->count; e++) {
    if(
      elves->elf[e]->now[0] >= min_x &&
      elves->elf[e]->now[0] <= max_x &&
      elves->elf[e]->now[1] >= min_y &&
      elves->elf[e]->now[1] <= max_y
    ) {
      total--;
    }
  }
  
  return total;
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  elves_t* elves = calloc(1, sizeof(elves_t));
  elves->count = 0;
  elves->elf = calloc(6000, sizeof(elf_t*));

  char* input = NULL;
  size_t bufsize = 0, x, y = 0;
  char c;
  elf_t* e;
  while(getline(&input, &bufsize, fptr) != -1) {
    for(x = 0; x < strlen(input); x++) {
      c = input[x];
      if(c == '#') {
        e = calloc(1, sizeof(elf_t));
        e->now[0] = (x + 1000);
        e->now[1] = (y + 1000);
        elves->elf[elves->count++] = e;
      }
    }
    y++;
  }
  free(input);
  fclose(fptr);
  
  uint8_t grid[2000][2000], moves[2000][2000];
  uint8_t d = 0, m;
  
  for(size_t i = 0; i < 10; i++) {
    // reset mid-round holders
    clear_grid(grid);
    clear_grid(moves);
    
    // fill grid with current elf positions
    for(size_t e = 0; e < elves->count; e++) {
      grid[elves->elf[e]->now[0]][elves->elf[e]->now[1]] = 1;
    }
    
    for(size_t e = 0; e < elves->count; e++) {
      if(
        (grid[elves->elf[e]->now[0]][elves->elf[e]->now[1] - 1] == 1) ||
        (grid[elves->elf[e]->now[0] + 1][elves->elf[e]->now[1] - 1] == 1) ||
        (grid[elves->elf[e]->now[0] + 1][elves->elf[e]->now[1]] == 1) ||
        (grid[elves->elf[e]->now[0] + 1][elves->elf[e]->now[1] + 1] == 1) ||
        (grid[elves->elf[e]->now[0]][elves->elf[e]->now[1] + 1] == 1) ||
        (grid[elves->elf[e]->now[0] - 1][elves->elf[e]->now[1] + 1] == 1) ||
        (grid[elves->elf[e]->now[0] - 1][elves->elf[e]->now[1]] == 1) ||
        (grid[elves->elf[e]->now[0] - 1][elves->elf[e]->now[1] - 1] == 1)
      ) {
        for(size_t i = 0; i < 4; i++) {
          m = (i + d) % 4;
          if(m == 0) { // look North
            if(
              (grid[elves->elf[e]->now[0] - 1][elves->elf[e]->now[1] - 1] == 0) &&
              (grid[elves->elf[e]->now[0]][elves->elf[e]->now[1] - 1] == 0) &&
              (grid[elves->elf[e]->now[0] + 1][elves->elf[e]->now[1] - 1] == 0)
            ) {
              moves[elves->elf[e]->now[0]][elves->elf[e]->now[1] - 1] += 1;
              elves->elf[e]->next[0] = elves->elf[e]->now[0];
              elves->elf[e]->next[1] = (elves->elf[e]->now[1] - 1);
              break;
            }
          } else if(m == 1) { // look South
            if(
              (grid[elves->elf[e]->now[0] - 1][elves->elf[e]->now[1] + 1] == 0) &&
              (grid[elves->elf[e]->now[0]][elves->elf[e]->now[1] + 1] == 0) &&
              (grid[elves->elf[e]->now[0] + 1][elves->elf[e]->now[1] + 1] == 0)
            ) {
              moves[elves->elf[e]->now[0]][elves->elf[e]->now[1] + 1] += 1;
              elves->elf[e]->next[0] = elves->elf[e]->now[0];
              elves->elf[e]->next[1] = (elves->elf[e]->now[1] + 1);
              break;
            }
          } else if(m == 2) { // look West
            if(
              (grid[elves->elf[e]->now[0] - 1][elves->elf[e]->now[1] - 1] == 0) &&
              (grid[elves->elf[e]->now[0] - 1][elves->elf[e]->now[1]] == 0) &&
              (grid[elves->elf[e]->now[0] - 1][elves->elf[e]->now[1] + 1] == 0)
            ) {
              moves[elves->elf[e]->now[0] - 1][elves->elf[e]->now[1]] += 1;
              elves->elf[e]->next[0] = (elves->elf[e]->now[0] - 1);
              elves->elf[e]->next[1] = elves->elf[e]->now[1];
              break;
            }
          } else if(m == 3) { // look East
            if(
              (grid[elves->elf[e]->now[0] + 1][elves->elf[e]->now[1] - 1] == 0) &&
              (grid[elves->elf[e]->now[0] + 1][elves->elf[e]->now[1]] == 0) &&
              (grid[elves->elf[e]->now[0] + 1][elves->elf[e]->now[1] + 1] == 0)
            ) {
              moves[elves->elf[e]->now[0] + 1][elves->elf[e]->now[1]] += 1;
              elves->elf[e]->next[0] = (elves->elf[e]->now[0] + 1);
              elves->elf[e]->next[1] = elves->elf[e]->now[1];
              break;
            }
          }
        }
      }
    }
  
    // finally, check if only one elf wants to make that move
    for(size_t e = 0; e < elves->count; e++) {    
      if(moves[elves->elf[e]->next[0]][elves->elf[e]->next[1]] == 1) {
        // printf("Elf %zu moved\n", (e + 1));
        elves->elf[e]->now[0] = elves->elf[e]->next[0];
        elves->elf[e]->now[1] = elves->elf[e]->next[1];
      }
    }
    
    d = (d + 1) % 4; // rotate check direction
  }

  uint32_t total = count_empty(elves);
  printf("Total number of empty spaces = %d\n", total); // 3996
  
  for(size_t e = 0; e < elves->count; e++) free(elves->elf[e]);
  free(elves->elf);
  free(elves);
  
  return 0;
}
