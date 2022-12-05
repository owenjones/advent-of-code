#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cratemover.h"

crate_t** init_stacks() {
  // return empty array to hold pointers to top crate in each stack
  crate_t** stacks = calloc(STACKS, sizeof(crate_t));
  for(size_t i = 0; i < STACKS; i++) {
    stacks[i] = NULL;
  }
  return stacks;
}

void fill_stacks(crate_t** stacks, FILE* fptr) {
  char* line = NULL;
  size_t size;
  int loop = 1;

  while((getline(&line, &size, fptr) != -1) && loop) {
    if(strchr(line, '[') == NULL) loop = 0; // stop when lines stop containing crates

    for(size_t i = 0; i < STACKS; i++) {
      char c = line[(i*4) + 1]; // crate letters appear at positions 1, 5, 9...
      if(c >= 'A' && c <= 'Z') { // letter = crate to be added to stack i, no letter = no crate present
        crate_t* n = calloc(1, sizeof(crate_t));
        n->id = c;
        n->next = NULL;

        if(stacks[i] == NULL) {
          // if stack is empty insert new crate at head
          stacks[i] = n;
        } else {
          // otherwise traverse list of crates to find end and insert there
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
  for(size_t i = 0; i < STACKS; i++) {
    // loop through stack array (pointers to top crate)
    printf("%lu => ", i);
    crate_t* ptr = stacks[i];
    while(ptr != NULL) {
      // traverse list of crates and print out crate id (reference letter)
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
    // loop through stack array (pointers to top crate)
    crate_t* ptr = stacks[i];
    crate_t* next;
    while(ptr != NULL) {
      // traverse list of crates, temp store pointer to next crate and free current crate
      next = ptr->next;
      free(ptr);
      ptr = next;
    }
  }
}

void move_crates_individually(crate_t** stacks, int n, int from, int to) {
  from -= 1; // correct indexes
  to -= 1;

  // Take top crate on 'from' stack and adjust pointers so that current
  // top of 'to' stack is it's child, and then place it on top of 'to' stack;
  // then update 'from' stack to place next child at the top (repeat n times).
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

  // Take top crate on 'from' stack and traverse n crates to get pointers
  // to the bottom crate, and the crate to be the new head of that stack.
  // Update pointers so that top of 'to' stack is now the child of the bottom
  // crate and the head crate is now the top of the 'to' stack, and the new
  // head crate is the top of the 'from' stack.
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
  // Traverse list of top crates and print out id (reference letter)
  printf("Top of stacks: ");
  for(size_t i = 0; i < STACKS; i++) {
    printf("%c", stacks[i]->id);
  }
  printf("\n");
}
