#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "filemanager.h"

int find_nodes(node_t* node) {
  int size = 0;
  for(size_t i = 0; i < node->nchildren; i++) {
    node_t* child = node->children[i];
    if(child->type == directory) {
      if(child->size <= 100000) size += child->size;
      size += find_nodes(child);
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

  uint32_t total = find_nodes(root);
  printf("Total size of directories = %u\n", total); // 1086293
  
  free_nodes(root);
  return 0;
}
