#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "filemanager.h"

void add_child(node_t* parent, node_t* child) {
  if(parent == NULL) return;
  parent->child[parent->children++] = child;
}

int find_child(node_t* node, char* id) {
  // find index to node in list of children (if it exists)
  for(size_t i = 0; i < node->children; i++) {
    if(strcmp(id, node->child[i]->id) == 0) return i;
  }

  return -1;
}

void update_sizes(node_t* node) {
  // propagate new node's size up the node tree (to update directory sizes)
  uint32_t size = node->size;
  node_t* ptr = node->parent;
  while(ptr != NULL) {
    ptr->size += size;
    ptr = ptr->parent;
  }
}

node_t* create_node(char* id, enum type type, uint32_t size, node_t* parent) {
  // create a new node as a child of the current active node (directory)

  // should really check here if a node with this id already exists
  // (even though we know it won't in the puzzle)

  node_t* node = calloc(1, sizeof(node_t));
  strcpy(node->id, id);
  node->type = type;
  node->size = size;
  node->parent = parent;
  node->child = calloc(32, sizeof(node_t*));
  node->children = 0;

  if(parent) add_child(parent, node);
  if(type == file) update_sizes(node);

  return node;
}

node_t* build_tree_from_file(char* fname) {
  FILE* fptr;
  if((fptr = fopen(fname, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  node_t* root = create_node("/", directory, 0, NULL);
  node_t* current = NULL;

  char* name = calloc(128, sizeof(char));
  uint32_t size;

  char* input = NULL;
  size_t bufsize = 0;
  while(getline(&input, &bufsize, fptr) != -1) {
    if(sscanf(input, "$ cd %s", name) > 0) {
      // 'cd' command (will ignore 'ls' and just handle files/directories read in)
      if(strcmp(name, "/") == 0) {
        // point current node to root of tree
        current = root;
      }
      else if(strcmp(name, "..") == 0) {
        // move up the tree to parent node
        if(current->parent != NULL) {
          current = current->parent;
        }
      } else {
        // move down the tree to node identified by directory name
        int child = find_child(current, name);
        if(child != -1) {
          current = current->child[child];
        }
      }
    } else if(sscanf(input, "dir %s", name) > 0) {
      // create a new directory node
      create_node(name, directory, 0, current);
    } else if(sscanf(input, "%u %s", &size, name)) {
      // create a new file node
      create_node(name, file, size, current);
    }
  }

  fclose(fptr);
  free(name);
  free(input);

  return root;
}

void free_nodes(node_t* node) {
  for(size_t i = 0; i < node->children; i++) {
    free_nodes(node->child[i]);
  }
  free(node->child);
  free(node);
}
