#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cratemover.h"

void init_stacks(crate_t** stacks) {
  for(size_t i = 0; i < STACKS; i++) {
    stacks[i] = NULL;
  }
}

void fill_stacks(crate_t** stacks, FILE* fptr) {
  char* line = NULL;
  size_t size;
  int loop = 1;
  
  while((getline(&line, &size, fptr) != -1) && loop) {
    if(strchr(line, '[') == NULL) loop = 0; // quit when lines stop containing crates
    
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
          while(ptr->next != NULL) {
            ptr = ptr->next;
          }
          ptr->next = n;
        }
      }
    }
  }
  
  free(line);
}

void print_stacks(crate_t** stacks) {
  #ifdef DEBUG
  // DEBUG: show crate structure
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
  #endif
}

void free_stacks(crate_t** stacks) {
  for(size_t i = 0; i < STACKS; i++) {
    crate_t* ptr = stacks[i];
    crate_t* next;
    while(ptr != NULL) {
      next = ptr->next;
      free(ptr);
      ptr = next;
    }
  }
}

void move_crates_individually(crate_t** stacks, int n, int from, int to) {
  from -= 1; // correct indexes
  to -= 1;
  
  for(size_t i = 0; i < n; i++) {
    crate_t* temp = stacks[from]->next;
    stacks[from]->next = stacks[to];
    stacks[to] = stacks[from];
    stacks[from] = temp;
  }
}

void move_crates_grouped(crate_t** stacks, int n, int from, int to) {
  from -= 1; // correct indexes
  to -= 1;
  
  crate_t* head = stacks[from];
  crate_t* nhead = head;
  crate_t* bottom = NULL;
  for(size_t i = 0; i < n; i++) {
    bottom = nhead;
    nhead = nhead->next;
  }
  
  stacks[from] = nhead;
  bottom->next = stacks[to];
  stacks[to] = head;
}


void get_top_crates(crate_t** stacks) {
  printf("Top of stacks: ");
  for(size_t i = 0; i < STACKS; i++) {
    printf("%c", stacks[i]->id);
  }
  printf("\n");
}
