#include <stdio.h>
#include <stdlib.h>

typedef struct entry {
  int value;
  struct entry* left;
  struct entry* right;
} entry_t;

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  entry_t *indexes[5000], *n;
  int x = 0, v;
  while(fscanf(fptr, "%d", &v) == 1) {
    n = calloc(1, sizeof(entry_t));
    n->value = v;

    if(x > 0) {
      n->left = indexes[x - 1];
      indexes[x - 1]->right = n;
    }
    
    indexes[x++] = n;
  }
  fclose(fptr);
  
  // close the loop
  indexes[0]->left = indexes[x - 1];
  indexes[x - 1]->right = indexes[0];
  
  entry_t *current, *left, *right, *insert;
  size_t i, j;
  for(i = 0; i < x; i++) {
    v = indexes[i]->value % (x - 1);    
    current = indexes[i];
    left = indexes[i]->left;
    right = indexes[i]->right;
    insert = indexes[i];

    // find position to insert element at
    if(v == 0) {
      continue;
    } else if(v > 0) {
      for(j = 0; j < v; j++) insert = insert->right;
    } else if(v < 0) {
      for(j = 0; j < abs(v); j++) insert = insert->left;
    }
    
    // cut entry from loop by joining left and right of neighbouring nodes together
    left->right = right;
    right->left = left;
    
    if(v > 0) {
      left = insert;
      right = insert->right;
    } else if(v < 0) {
      left = insert->left;
      right = insert;
    }
    
    // re-insert entry in new position
    left->right = current;
    current->left = left;
    
    right->left = current;
    current->right = right;
  }

  // find start point (position where value = 0)
  entry_t* ptr;
  for(i = 0; i < x; i++) {
    if(indexes[i]->value == 0) {
      ptr = indexes[i];
      // break;
    }
  }

  // iterate over loop to get values that fall at 1000th, 2000th, and 3000th positions
  int total = 0;
  i = 0;
  while(i <= 3000) {
    if(i == 1000 || i == 2000 || i == 3000) {
      total += ptr->value;
    }
    ptr = ptr->right;
    i++;
  }
  for(i = 0; i < x; i++) free(indexes[i]);

  printf("Total: %d\n", total); // 13183

  return 0;
}
