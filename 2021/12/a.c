#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum size{Big, Small, Exit};

typedef struct cave {
  char id[6];
  int hash;
  enum size size;
  struct cave* paths[10];
  int npaths;
} cave_t;

void split(char* in, char* left, char* right) {
  for(size_t i = 0; i < strlen(in); i++) {
    if(in[i] == '-') {
      strncpy(left, in, i);
      strncpy(right, &in[i+1], (strlen(in) - i - 1));
      return;
    }
  }
}

int hash(char* id) {
  int h = 0;
  for(size_t i = 0; i < strlen(id); i++) h += ((int) id[i] - 65);
  return h;
}

void cave_size(cave_t* cave) {
  if(cave->hash <= 50) {
    cave->size = Big;
  } else if(cave->hash <= 114) {
    cave->size = Small;
  } else {
    cave->size = Exit; // start/end nodes
  }
}

cave_t* get_cave(cave_t** caves, char* id) {
  int h = hash(id);
  if(caves[h] == 0) {
    cave_t* cave = (cave_t*) calloc(1, sizeof(cave_t));
    strncpy(cave->id, id, strlen(id));
    cave->hash = h;
    cave_size(cave);
    caves[h] = cave;
  }

  return caves[h];
}

void free_caves(cave_t** caves) {
  for(size_t i = 0; i < 234; i++) {
    if(caves[i] != 0) free(caves[i]);
  }

  free(caves);
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  cave_t **caves = (cave_t**) calloc(234, sizeof(cave_t*));
  char* in = (char*) calloc(10, sizeof(char));
  while(fscanf(fptr, "%s", in) > 0) {
    char *left = (char*) calloc(6, sizeof(char));
    char *right = (char*) calloc(6, sizeof(char));
    split(in, left, right);
    
    cave_t* l = get_cave(caves, left);
    cave_t* r = get_cave(caves, right);
    l->paths[l->npaths++] = r;
    r->paths[r->npaths++] = l;

    free(left);
    free(right);
  }
  free(in);
  fclose(fptr);

  cave_t* start = get_cave(caves, "start");
  cave_t* end = get_cave(caves, "end");

  // walk the cave - find start node (use hash func), then iteratively
  // access each node in paths array and walk from there
  // - how to track small caves in this to prevent re-visiting?
  // ^ check on the visit, or generate all paths and parse at the end to remove


  int paths = 0;
  printf("Paths through the system: %i\n", paths);

  free_caves(caves);
  return 0;
}
