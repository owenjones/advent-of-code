#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

enum type { directory, file };

typedef struct node {
  char id[128];
  enum type type;
  uint32_t size;
  struct node* parent;
  int nchildren;
  struct node** children;
} node_t;

void add_child(node_t* parent, node_t* child) {
  if(parent == NULL) return;
  parent->children[parent->nchildren] = child;
  parent->nchildren++;
}

int find_child(node_t* node, char* id) {
  for(size_t i = 0; i < node->nchildren; i++) {
    if(strcmp(id, node->children[i]->id) == 0) return i;
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
  node->children = calloc(128, sizeof(node_t*));
  node->nchildren = 0;

  add_child(parent, node);
  if(type == file) update_sizes(node);
  return node;
}

int walk_tree(node_t* node) {
  int size = 0;
  for(size_t i = 0; i < node->nchildren; i++) {
    node_t* child = node->children[i];
    if(child->type == directory) {
      if(child->size <= 100000) size += child->size;
      size += walk_tree(child);
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
  node_t* current = root;
  
  char* input = calloc(128, sizeof(char));
  size_t size;
  while(getline(&input, &size, fptr) != -1) {
    if(strcmp(input, "$ cd /\n") == 0) continue; // skip entering root directory
    
    char* name = calloc(128, sizeof(char));
    uint32_t size;
    if(sscanf(input, "$ cd %s", name) > 0) {
      // 'cd' command => will skip over ls commands to any file/directory entries
      if(strcmp(name, "..") == 0) {
        //printf("Move back a directory\n");
        current = current->parent;
      } else {
        //printf("Change directory to %s\n", name);
        int child = find_child(current, name);
        if(child < 0) {
          printf("!! Node '%s' doesn't exist !!\n", name);
          exit(1);
        }
        current = current->children[child];
      }
    } else if(sscanf(input, "dir %s", name) > 0) {
      // we have a directory
      //printf("New directory: %s\n", name);
      create_node(name, directory, 0, current);
    } else if(sscanf(input, "%u %s", &size, name)) {
      // we have a file
      //printf("New file: %s\n", name);
      create_node(name, file, size, current);
    }
    free(name);
  }
  fclose(fptr);
  free(input);

  uint32_t total = walk_tree(root);
  printf("Total size of directories = %u\n", total);
  
  return 0;
}
