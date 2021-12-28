#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum size{Big, Small, Special};

typedef struct cave {
  char id[6];
  int hash;
  enum size size;
  struct cave* paths[10];
  int npaths;
} cave_t;

typedef struct state {
  int paths;
  cave_t** valid_paths[200000];
  cave_t** path;
  int depth;
  int* direction;
  int saved;
  int saved_depth;
  int triggered;
  int triggered_depth;
} state_t;

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

state_t* init_state(cave_t* start) {
  state_t* state = (state_t*) calloc(1, sizeof(state_t));
  state->paths = 0;
  state->path = (cave_t**) calloc(50, sizeof(cave_t*));
  state->path[0] = start;
  state->depth = 0;
  state->direction = (int*) calloc(50, sizeof(int));
  state->saved = 0;
  state->saved_depth = 0;
  state->triggered = 0;
  state->triggered_depth = 0;
  return state;
}

cave_t* current_cave(state_t* state) {
  return state->path[state->depth];
}

cave_t* next_cave(state_t* state) {
  return current_cave(state)->paths[state->direction[state->depth]];
}

void take_path(state_t* state) {
  state->path[(state->depth + 1)] = next_cave(state);
  state->depth++;
}

void skip_path(state_t* state) {
  state->direction[state->depth]++;
}

int is_valid_path(state_t* state) {
  return state->direction[state->depth] < state->path[state->depth]->npaths;
}

void backtrack(state_t* state) {
  state->direction[state->depth] = 0;
  state->depth--;
  
  if(state->depth < 0) return;
  
  if(next_cave(state)->hash == state->saved) {
    if(state->saved_depth == state->depth && !state->triggered) {
      state->saved = 0;
      state->depth++;
      return;
    } else if(state->triggered_depth == state->depth && state->triggered) {
      state->triggered = 0;
    }
  }
  
  state->path[state->depth+1] = NULL;
  state->direction[state->depth]++;
}

cave_t* get_cave(cave_t** caves, char* id) {
  int h = hash(id);

  if(caves[h] == 0) {
    cave_t* cave = (cave_t*) calloc(1, sizeof(cave_t));
    strncpy(cave->id, id, strlen(id));
    cave->hash = h;

    if(strcmp(cave->id, "start") == 0 || strcmp(cave->id, "end") == 0) cave->size = Special;
    else if (isupper(cave->id[0])) cave->size = Big;
    else if (islower(cave->id[0])) cave->size = Small;

    caves[h] = cave;
  }

  return caves[h];
}

int have_visited(state_t* state) {
  for(size_t i = 0; i < state->depth; i++) {
    if(state->path[i] == next_cave(state)) return 1;
  }
  return 0;
}

int is_unique_path(state_t* state) {
  for(size_t p = 0; p < state->paths; p++) {
    int unique = 0;
    for(size_t i = 0; i < 50; i++) {
      if(state->path[i] != state->valid_paths[p][i]) {
        unique = 1;
        break;
      }
    }
    if(!unique) return 0;
  }
  return 1;
}

void store_valid_path(state_t* state) {
  state->valid_paths[state->paths] = (cave_t**) calloc(50, sizeof(cave_t*));
  memcpy(state->valid_paths[state->paths], state->path, 50*sizeof(cave_t*));
  state->paths++;
}

void print_path(state_t* state) {
  for(size_t i = 0; i <= state->depth; i++) printf("%s,", state->path[i]->id);
  printf("end\n");
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
  cave_t *current = NULL, *next = NULL;
  state_t* state = init_state(start);

  while(state->depth >= 0) {
    if(!is_valid_path(state)) backtrack(state);
    else {
      current = current_cave(state);
      next = next_cave(state);

      if(next == end) {
        if(is_unique_path(state)) {
          print_path(state);
          store_valid_path(state);
        }
        
        skip_path(state);
      }
      else if(next == start) {
        skip_path(state);
      }
      else if(next->size == Small) {
        if(!have_visited(state)) {
          if(!state->saved) {
            state->saved = next->hash;
            state->saved_depth = state->depth;
          }
      
          take_path(state);
        }
        else if(state->saved && state->saved == next->hash && !state->triggered) {
          state->triggered = 1;
          state->triggered_depth = state->depth;
          take_path(state);
        }
        else {
          skip_path(state);
        }
      }
      else {
        take_path(state);
      }
      
    }
  }

  int paths = state->paths;
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
  load_caves("input.txt", caves);
  int paths = walk_caves(caves);
  free_caves(caves);
  printf("\nTotal paths through the system: %i\n", paths); // 94849
  return 0;
}
