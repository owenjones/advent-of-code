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

typedef struct state {
  int depth;
  cave_t** paths;
  int* direction;
  int saved;
  int sdepth;
  int triggered;
  int tdepth;
} state_t;

state_t* init_state(cave_t* start) {
  state_t* state = (state_t*) calloc(1, sizeof(state_t));
  state->depth = 0;
  state->paths = (cave_t**) calloc(50, sizeof(cave_t*));
  state->paths[0] = start;
  state->direction = (int*) calloc(50, sizeof(int));
  state->saved = 0;
  state->sdepth = 0;
  state->triggered = 0;
  state->tdepth = 0;
  return state;
}

cave_t* current_cave(state_t* state) {
  return state->paths[state->depth];
}

cave_t* next_cave(state_t* state) {
  return current_cave(state)->paths[state->direction[state->depth]];
}

void take_path(state_t* state) {
  state->paths[(state->depth + 1)] = next_cave(state);
  state->depth++;
}

void skip_path(state_t* state) {
  state->direction[state->depth]++;
}

int is_valid_path(state_t* state) {
  return state->direction[state->depth] < state->paths[state->depth]->npaths;
}

void backtrack(state_t* state) {
  printf("Backtracking... ");
  state->direction[state->depth] = 0;
  state->depth--;
  
  if(next_cave(state)->hash == state->saved && state->triggered && state->tdepth == state->depth) {
    printf("and untriggering this small cave\n");
    state->triggered = 0;
  } else if(next_cave(state)->hash == state->saved && state->sdepth == state->depth) {
    printf("and unsaving this small cave\n");
    state->saved = 0;
    state->sdepth = 0;
    state->triggered = 0;
  }
  
  if(state->depth > 0) state->direction[state->depth]++;
}

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

cave_t* get_cave(cave_t** caves, char* id) {
  int h = hash(id);

  if(caves[h] == 0) {
    cave_t* cave = (cave_t*) calloc(1, sizeof(cave_t));
    strncpy(cave->id, id, strlen(id));
    cave->hash = h;

    if(cave->hash <= 50) { // 50==ZZ
      cave->size = Big;
    } else if(cave->hash <= 114) { // 114==zz
      cave->size = Small;
    } else {
      cave->size = Special; // start/end nodes
    }

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

void print_path(state_t* state) {
  for(size_t i = 0; i <= state->depth; i++) printf("%s,", state->paths[i]->id);
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
  int paths = 0;
  cave_t* start = get_cave(caves, "start");
  cave_t* end = get_cave(caves, "end");
  cave_t *current = NULL, *next = NULL;
  state_t* state = init_state(start);

  while(state->depth >= 0) {
    if(!is_valid_path(state)) backtrack(state);
    else {
      current = current_cave(state);
      next = next_cave(state);

      printf("In cave <%s>, next cave is <%s> (path %i): ", current->id, next->id, state->direction[state->depth]);

      if(next == NULL) {
        printf("ERROR - next cave is a null pointer\n");
        exit(1);
      }

      if(next == end) {
        printf("we've reached the end\n");
        paths++;
        printf("%4i - ", paths);
        print_path(state);
        skip_path(state);
      }
      else if(next == start) {
        printf("this would take us back to the start\n");
        skip_path(state);
      }
      else if(next->size == Small) {
        printf("this is a small cave - ");
        if(!state->saved) {
          printf("saved for a re-visit\n");
          state->saved = next->hash;
          state->sdepth = state->depth;
          take_path(state);
        }
        else if(state->saved == next->hash && !state->triggered) {
          printf("our single allowed re-visit\n");
          state->triggered = 1;
          state->tdepth = state->depth;
          take_path(state);
        }
        else if(!have_visited(state->paths, state->depth, next)) {
          printf("we haven't visited it yet\n");
          take_path(state);
        }
        else {
          printf("we can't go this way\n");
          skip_path(state);
        }
      }
      else {
        printf("let's take this path\n");
        take_path(state);
      }
      
      if(state->depth > 15) exit(1);
    }
  }

  free(state);
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
