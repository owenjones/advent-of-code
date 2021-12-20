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

typedef struct save {
  int hash;
  int depth;
  int triggered;
  int tdepth;
} save_t;

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

int have_visited(cave_t** path, int depth, cave_t* cave) {
  for(size_t i = 0; i < depth; i++) {
    if(path[i] == cave) return 1;
  }
  return 0;
}

void print_path(cave_t** path, int depth) {
  for(size_t i = 0; i <= depth; i++) printf("%s,", path[i]->id);
  printf("end\n\n");
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
  cave_t** path = (cave_t**) calloc(50, sizeof(cave_t));
  int* direction = (int*) calloc(50, sizeof(int));
  cave_t* next;
  save_t* save = (save_t*) calloc(1, sizeof(save_t));
  
  save->hash = 0;
  path[0] = start;

  while(depth >= 0) {
    printf("In cave <%s> (depth=%i)\n", path[depth]->id, depth);
    
    if(direction[depth] > (path[depth]->npaths - 1)) {
      direction[depth] = 0;
      
      if(save->hash && save->depth == depth && save->hash == path[depth]->hash) {
        printf("we're back to the small cave we're allowed to visit first\n");
        save->hash = 0;
      } else if(save->hash && save->hash == path[depth]->hash && save->triggered && save->tdepth == depth) {
        save->triggered = 0;
      } else {
        printf("no more valid paths forward, lets go back one\n");
        path[depth] = NULL;
        depth--;
        if(depth >= 0) direction[depth]++;
        // if(depth == 0) save->hash = 0;
      }
      
    } else {
      next = path[depth]->paths[direction[depth]];

      if(next == end) {
        printf("we've reached the end\n");
        paths++;
        printf("%4i - ", paths);
        print_path(path, depth);
        direction[depth]++;
      } else if(next->size == Small) {
        printf("we've reached a small cave <%s>\n", next->id);
        if(!save->hash) {
          printf("this is the first small cave we've visited\n");
          save->hash = next->hash;
          save->depth = depth;
          depth++;
          path[depth] = next;
        } else {
          printf("this is not the first small cave we've visited ");
          if(save->hash == next->hash && save->triggered == 0) {
            printf("but we can visit this one again\n");
            save->triggered = 1;
            save->tdepth = depth;
            depth++;
            path[depth] = next;
          } else if(!have_visited(path, depth, next)) {
            depth++;
            path[depth] = next;
            printf("but we haven't visited this cave yet\n");
          } else {
            printf("and we can't take it again, so we can't take this path\n");
            direction[depth]++;
          }
        }
      } else if(next == start) {
        direction[depth]++;
        printf("this path would take us back to the beginning\n");
      } else {
        printf("lets go into this cave <%s>\n", next->id);
        depth++;
        path[depth] = next;
      }
    }
  }

  free(direction);
  free(path);

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
  load_caves("test_input_c.txt", caves);
  int paths = walk_caves(caves);
  free_caves(caves);
  printf("\nTotal paths through the system: %i\n", paths); // 3495
  return 0;
}
