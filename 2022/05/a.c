#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "input.txt"
#define STACKS 9

typedef struct crate {
  char id;
  struct crate* next;
} crate_t;

int main(void) {
  char file[] = INPUT;
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  crate_t* stacks[STACKS];
  for(size_t i = 0; i < STACKS; i++) {
    stacks[i] = NULL;
  }
  
  char* line = NULL;
  size_t len;
  int loop = 1;
  
  // fill stack structure with crates
  while((getline(&line, &len, fptr) != -1) && loop) {
    if(strcmp(line, " 1   2   3   4   5   6   7   8   9 \n") == 0) loop = 0;
    //if(strcmp(line, " 1   2   3 \n") == 0) loop = 0;
    
    for(size_t i = 0; i < STACKS; i++) {
      char c = line[(i*4) + 1];
      if((int) c > 64 && (int) c < 91) {
        crate_t* n = calloc(1, sizeof(crate_t));
        n->id = c;
        n->next = NULL;
        
        if(stacks[i] == NULL) {
          stacks[i] = n;
        } else {
          crate_t* ptr = stacks[i];
          crate_t* parent = NULL;
          while(ptr != NULL) {
            parent = ptr;
            ptr = parent->next;
          }
          parent->next = n;
        }
      }
    }
  }
  
  int n, a, b;
  while(fscanf(fptr, "move %i from %i to %i\n", &n, &a, &b) > 0) {
    // DEBUG: show crate structure;
    for(size_t i = 0; i < STACKS; i++) {
      printf("%lu => ", i);
      crate_t* ptr = stacks[i];
      while(ptr != NULL) {
        printf("%c ", ptr->id);
        ptr = ptr->next;
      }
      printf("\n");
    }
    printf("\n");
    
    a -= 1; // correct indexes
    b -= 1;
    
    for(size_t i = 0; i < n; i++) {
      crate_t* temp = stacks[a]->next;
      stacks[a]->next = stacks[b];
      stacks[b] = stacks[a];
      stacks[a] = temp;
    }
  }
  
  

  fclose(fptr);

  printf("Top of stacks: ");
  for(size_t i = 0; i < STACKS; i++) {
    printf("%c", stacks[i]->id);
  }
  printf("\n");
  
  return 0;
}
