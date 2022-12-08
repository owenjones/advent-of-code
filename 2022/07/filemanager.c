#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "filemanager.h"

void add_child(node_t* parent, node_t* child) {
  if(parent == NULL) return;
  parent->child[parent->children] = child;
  parent->children++;
}

int find_child(node_t* node, char* id) {
  for(size_t i = 0; i < node->children; i++) {
    if(strcmp(id, node->child[i]->id) == 0) return i;
  }
  return -1;
}

void update_sizes(node_t* node) {
  // propagate new node's size up the node tree to update directory sizes
  uint32_t size = node->size;
  node_t* ptr = node->parent;
  while(ptr != NULL) {
    ptr->size += size;
    ptr = ptr->parent;
  }
}

node_t* create_node(char* id, enum type type, uint32_t size, node_t* parent) {
  // create a new node as a child of the current active directory
  node_t* node = calloc(1, sizeof(node_t));
  strcpy(node->id, id);
  node->type = type;
  node->size = size;
  node->parent = parent;
  node->child = calloc(128, sizeof(node_t*));
  node->children = 0;

  add_child(parent, node);
  if(type == file) update_sizes(node);
  return node;
}

void build_tree(FILE* fptr, node_t* root) {
  node_t* current = root;
  char* name = calloc(128, sizeof(char));
  uint32_t size;

  char* input = NULL;
  size_t bufsize = 0;
  while(getline(&input, &bufsize, fptr) != -1) {
    // skip entering root directory as we've already made it
    if(strcmp(input, "$ cd /\n") == 0) continue;
    if(sscanf(input, "$ cd %s", name) > 0) {
      // 'cd' command => will ignore 'ls'
      if(strcmp(name, "..") == 0) {
        // move up the tree to parent node
        current = current->parent;
      } else {
        // move down the tree to node identified by directory name
        current = current->child[find_child(current, name)];
      }
    } else if(sscanf(input, "dir %s", name) > 0) {
      // create a new directory node
      create_node(name, directory, 0, current);
    } else if(sscanf(input, "%u %s", &size, name)) {
      // create a new file node
      create_node(name, file, size, current);
    }
  }
  free(name);
  free(input);
}

void free_nodes(node_t* node) {
  for(size_t i = 0; i < node->children; i++) {
    free_nodes(node->child[i]);
  }
  free(node->child);
  free(node);
}
