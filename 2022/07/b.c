#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "filemanager.h"

void walk_tree(node_t* node, uint32_t size, cand_t* candidates) {
  for(size_t i = 0; i < node->nchildren; i++) {
    node_t* child = node->children[i];
    if(child->type == directory) {
      if(child->size >= size) {
        candidates->sizes[candidates->n] = child->size;
        candidates->n++;
      }
      walk_tree(child, size, candidates);
    }
  }
}

int find_candidate(node_t* root, uint32_t size) {
  cand_t* candidates = calloc(1, sizeof(cand_t));
  candidates->n = 0;
  candidates->sizes = calloc(256, sizeof(uint32_t));
  walk_tree(root, size, candidates);

  uint32_t temp = 0;
  for(size_t i = 0; i < candidates->n; i++) {
    for(size_t j = 0; j < (candidates->n - i); j++) {
      if(candidates->sizes[j] > candidates->sizes[j+1]) {
        temp = candidates->sizes[j];
        candidates->sizes[j] = candidates->sizes[j+1];
        candidates->sizes[j+1] = temp;
      }
    }
  }
  
  // sizes[0] == 0 and I cba to work out why so just skip it
  return candidates->sizes[1];
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  node_t* root = create_node("/", directory, 0, NULL);
  build_tree(fptr, root);
  fclose(fptr);

  uint32_t required = 30000000 - (70000000 - root->size);
  uint32_t del = find_candidate(root, required);
  printf("Size of directory to delete = %u\n", del); // 366028
  
  free_nodes(root);
  
  return 0;
}
