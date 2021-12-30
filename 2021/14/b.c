#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct pair {
  int left;
  int right;
} pair_t;

int hash_letter(char letter) {
  // convert single letter into hash value
  return (letter - 65);
}

int hash_pair(char pair[2]) {
  // convert letter pair into hash value
  return ((pair[0] - 65) << 8) + (pair[1] - 65);
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  char* template = calloc(21, sizeof(char));
  fscanf(fptr, "%s\n\n", template);

  pair_t **pairs = calloc(6426, sizeof(pair_t*));
  pair_t **rules = calloc(6426, sizeof(pair_t*));
  
  int h;
  pair_t *p, *r;
  char pair[3], insert[2], left[3], right[3];
  while(fscanf(fptr, "%2c -> %1c\n", pair, insert) > 0) {
    h = hash_pair(pair);

    p = calloc(1, sizeof(pair_t));
    p->left = hash_letter(pair[0]);
    p->right = hash_letter(pair[1]);
    pairs[h] = p;
    
    left[0] = pair[0];
    left[1] = *insert;
    
    right[0] = *insert;
    right[1] = pair[1];

    r = calloc(1, sizeof(pair_t));
    r->left = hash_pair(left);
    r->right = hash_pair(right);
    rules[h] = r;
  }
  
  fclose(fptr);
    
  uint64_t *counts = calloc(6426, sizeof(uint64_t));
  uint64_t *tcounts;
  
  // Make initial count of all pairs in template
  for(size_t i = 0; i < strlen(template); i++) {
    pair[0] = template[i];
    pair[1] = template[i+1];
    counts[hash_pair(pair)]++;
  }
  
  for(size_t s = 0; s < 40; s++) {
    tcounts = calloc(6426, sizeof(uint64_t));
    
    // For each step - take count of pair and add pairs the character
    // insertion would generate if we were folding normally
    for(size_t i = 0; i < 6426; i++) {
      if(rules[i] != NULL) {
        tcounts[rules[i]->left] += counts[i];
        tcounts[rules[i]->right] += counts[i];
      }
    }
    
    free(counts);
    counts = tcounts;
  }
  
  // Count the frequency of each letter in the pairs
  uint64_t *letters = calloc(25, sizeof(uint64_t));
  for(size_t i = 0; i < 6426; i++) {
    if(pairs[i] != NULL) {
      letters[pairs[i]->left] += counts[i];
      letters[pairs[i]->right] += counts[i];
    }
    
    free(pairs[i]);
    free(rules[i]);
  }
  free(counts);

  // Need to add on first & last letters of template as these only
  // get covered by one pair (all other letters appear in both left and right of pairs)
  letters[hash_letter(template[0])]++;
  letters[hash_letter(template[(strlen(template) - 1)])]++;
  free(template);

  // Sort counts largest to smallest
  size_t i = 0;
  uint64_t temp;
  while(i < 25) {
    if(letters[i] < letters[i+1]) {
      temp = letters[i];
      letters[i] = letters[i+1];
      letters[i+1] = temp;
      i = 0;
    } else {
      i++;
    }
  }

  // Find last non-zero count, and subtract from highest count - need to halve
  // amount (as letters appear twice - in left of one pair, and right of another)
  size_t x = 0;
  while(letters[x] > 0) x++;
  uint64_t answer = (letters[0] - letters[(x-1)]) / 2; // 4110215602456
  free(letters);
  
  printf("Frequency of most common element minus frequency of least common: %llu\n", answer);
  return 0;
}
