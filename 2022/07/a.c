#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "filemanager.h"

uint32_t find_sum_of_nodes(node_t* node, uint32_t max) {
  uint32_t size = 0;
  for(size_t i = 0; i < node->children; i++) {
    node_t* child = node->child[i];
    if(child->type == directory) {
      if(child->size <= max) size += child->size;
      size += find_sum_of_nodes(child, max);
    }
  }
  return size;
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

  uint32_t total = find_sum_of_nodes(root, 100000);
  printf("Total size of directories = %u\n", total); // 1086293

  free_nodes(root);
  return 0;
}
