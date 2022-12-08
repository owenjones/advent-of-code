#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "filemanager.h"

typedef struct candidates {
  uint8_t n;
  uint32_t* sizes;
} cand_t;

void extract_candidates(cand_t* candidates, uint32_t size, node_t* node) {
  // recursively walk tree and extract any directory sizes which are >=
  // the required size, add these to the array of candidates
  for(size_t i = 0; i < node->children; i++) {
    node_t* child = node->child[i];
    if(child->type == directory) {
      if(child->size >= size) {
        candidates->sizes[candidates->n++] = child->size;
      }
      extract_candidates(candidates, size, child);
    }
  }
}

int find_best_candidate(node_t* root, uint32_t size) {
  cand_t* candidates = calloc(1, sizeof(cand_t));
  candidates->n = 0;
  candidates->sizes = calloc(256, sizeof(uint32_t));

  extract_candidates(candidates, size, root);

  // sort array of candidates smallest => largest
  uint32_t temp = 0;
  for(size_t i = 0; i < (candidates->n - 1); i++) {
    for(size_t j = 0; j < (candidates->n - 1 - i); j++) {
      if(candidates->sizes[j] > candidates->sizes[j+1]) {
        temp = candidates->sizes[j];
        candidates->sizes[j] = candidates->sizes[j+1];
        candidates->sizes[j+1] = temp;
      }
    }
  }

  int s = candidates->sizes[0];
  free(candidates->sizes);
  free(candidates);
  return s;
}

int main(void) {
  node_t* root = build_tree_from_file("input.txt");
  uint32_t required = 30000000 - (70000000 - root->size);
  uint32_t del = find_best_candidate(root, required);
  printf("Size of directory to delete = %u\n", del); // 366028

  free_nodes(root);
  return 0;
}
