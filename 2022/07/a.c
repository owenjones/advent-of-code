#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "filemanager.h"

uint32_t find_sum_of_nodes(node_t* node, uint32_t max) {
  uint32_t size = 0;
  
  for(size_t i = 0; i < node->children; i++) {
    if(node->child[i]->type == directory) {
      if(node->child[i]->size <= max) size += node->child[i]->size;
      size += find_sum_of_nodes(node->child[i], max);
    }
  }

  return size;
}

int main(void) {
  node_t* root = build_tree_from_file("input.txt");
  uint32_t total = find_sum_of_nodes(root, 100000);
  printf("Total size of directories = %u\n", total); // 1086293

  free_nodes(root);
  return 0;
}
