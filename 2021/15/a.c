#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM 10
#define INPUT "test_input.txt"

typedef struct cell {
  int x, y, cost;
} cell_t;

typedef struct heap {
  int size;
  int max;
  cell_t** cells;
} heap_t;

heap_t* heap_init(int size) {
  heap_t* heap = calloc(1, sizeof(heap_t));
  heap->max = size;
  heap->size = 0;
  heap->cells = calloc(size, sizeof(cell_t*));
  return heap;
}

cell_t* heap_min(heap_t* heap) {
  return heap->cells[0];
}

cell_t* heap_max(heap_t* heap) {
  return heap->cells[(heap->size - 1)];
}

void heap_heapify(heap_t* heap) {
  
}

void heap_insert(heap_t* heap, cell_t* cell) {
  heap->cells[heap->size++] = cell;
  heap_heapify(heap);
}

cell_t* heap_pop(heap_t* heap) {
  cell_t* cell = heap->cells[0];
  heap->cells[0] = NULL;
  heap_heapify(heap);
  return cell;
}

void heap_swap(heap_t* heap, int a, int b) {
  cell_t* temp = heap->cells[a];
  heap->cells[a] = heap->cells[b];
  heap->cells[b] = temp;
}

int heap_parent(int node) {
  return ((node - 1) / 2);
}

int heap_left_node(int parent) {
  return (2 * parent);
}

int heap_right_node(int parent) {
  return ((2 * parent) + 1);
}

void heap_print(heap_t* heap) {
  for(size_t i = 0; i < heap->size; i++) printf("%i ", heap->cells[i]->cost);
  printf("\n");
}

int hash(int x, int y) {
  if(x >= 0 && x < DIM && y >= 0 && y < DIM) return x + (DIM * y);
  return -1;
}

void neighbours(cell_t *c, int *n) {
  n[0] = hash(c->x + 1, c->y);
  n[1] = hash(c->x, c->y + 1);
  n[2] = hash(c->x - 1, c->y);
  n[3] = hash(c->x, c->y - 1);
}

int distance(cell_t *c) {
  return abs((DIM - 1) - c->x) + abs((DIM - 1) - c->y);
}

int main(void) {
  char file[] = INPUT;
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  cell_t **cells = calloc((DIM * DIM), sizeof(cell_t*));
  int cost;
  for(size_t i = 0; i < (DIM * DIM); i++) {
    fscanf(fptr, "%1i", &cost);
    cells[i] = calloc(1, sizeof(cell_t));
    cells[i]->x = i % DIM;
    cells[i]->y = i / DIM;
    cells[i]->cost = cost;
  }
  fclose(fptr);
  
  printf("Test: (%i,%i) = %i\n", cells[0]->x, cells[0]->y, cells[0]->cost);
  
  heap_t* frontier = heap_init(1000);
  cells[0]->cost = 0; // hack to stop start cost being included in total cost
  heap_insert(frontier, cells[0]);
  
  cell_t *target = cells[hash((DIM - 1), (DIM - 1))];
  int end = 0, n[4];
  
  // while(!end) {
  //   for(size_t i = 0; i < frontier->size; i++) {
  //     neighbours(frontier->cells[i], n);
  //     for(i = 0; i < 4; i++) {
  //       // if(n[i] > 0) ;
  //     }
  //   }
  // 
  //   // if(c == target) end = 1;
  // }
  
  for(size_t i = 0; i < (DIM * DIM); i++) free(cells[i]);
  free(cells);
  
  cell_t* answer = heap_min(frontier);
  printf("Lowest total risk: %i (%i, %i)\n", answer->cost, answer->x, answer->y);
  
  for(size_t i = 0; i < (frontier->size - 1); i++) free(frontier->cells[i]);
  free(frontier);
  
  return 0;
}
