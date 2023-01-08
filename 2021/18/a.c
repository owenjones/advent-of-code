#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

enum pair_type { UNKNOWN, PAIR, NUMBER };

typedef struct pair {
  enum pair_type type;
  struct pair *parent, *left, *right;
  int8_t number;
  uint8_t depth;
  int32_t magnitude;
} pair_t;

pair_t* new_pair(pair_t* parent) {
  pair_t* pair = calloc(1, sizeof(pair_t));
  pair->type = UNKNOWN;
  pair->parent = parent;
  pair->left = NULL;
  pair->right = NULL;
  pair->number = 0;
  pair->depth = 0;
  pair->magnitude = 0;
  return pair;
}

pair_t* add_pairs(pair_t* left, pair_t* right) {
  pair_t* pair = new_pair(left->parent);
  pair->type = PAIR;
  pair->left = left;
  pair->right = right;
  left->parent = pair;
  right->parent = pair;
  return pair;
}

void reduce_pairs(pair_t* pair) {
  
}

void explode_pair(pair_t* pair) {
  if(pair->parent == NULL) return;

}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("test_input_1.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  fclose(fptr);

  printf("");
  return 0;
}
