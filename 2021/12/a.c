#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum size{Big, Small, Special};

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
  if(cave->hash <= 50) { // 50==ZZ
    cave->size = Big;
  } else if(cave->hash <= 114) { // 114==zz
    cave->size = Small;
  } else {
    cave->size = Special; // start/end nodes
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

void load_caves(char* file, cave_t** caves) {
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

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
}

int walk_caves(cave_t** caves) {
  cave_t* start = get_cave(caves, "start");
  cave_t* end = get_cave(caves, "end");

  int paths = 0, depth = 0;
  int* taken = (int*) calloc(234, sizeof(int));
  int* dir = (int*) calloc(10, sizeof(int));
  cave_t *current = start, *next, *last;

  while(depth >= 0) {
    printf("Currently in cave <%s>\n", current->id);
    next = current->paths[dir[depth]];

    if(next == end) {
      // check if next step takes us to the end
      paths++;
      dir[depth]++;
    } else if(next->size == Small && taken[next->hash] == 1) {
      // check if next step takes us into a small cave we've already visited
      dir[depth]++;
    } else if(dir[depth] < current->npaths) {
      if(next->size == Small) taken[next->hash] = 1;
      last = current;
      current = next;
      depth++;
    } else {
      depth--;
      current = last;
    }

    if(depth == 0) {
      free(taken);
      taken = (int*) calloc(234, sizeof(int));
    }
  }

  printf("Starting point has %i possible routes\n", start->npaths);

  return paths;
}

void free_caves(cave_t** caves) {
  for(size_t i = 0; i < 234; i++) {
    if(caves[i] != 0) free(caves[i]);
  }

  free(caves);
}

int main(void) {
  cave_t **caves = (cave_t**) calloc(234, sizeof(cave_t*));
  load_caves("test_input.txt", caves);
  int paths = walk_caves(caves);
  free_caves(caves);
  printf("Paths through the system: %i\n", paths);
  return 0;
}
