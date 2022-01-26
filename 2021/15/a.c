#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM 100
#define INPUT "input.txt"

typedef struct cell {
  int x, y, cost;
} cell_t;

typedef struct heap {
  int size;
  int capacity;
  cell_t** cells;
} heap_t;

int heap_parent(int node) {
  return ((node - 1) / 2);
}

int heap_left_node(int parent) {
  return ((2 * parent) + 1);
}

int heap_right_node(int parent) {
  return ((2 * parent) + 2);
}

void heap_swap(heap_t* heap, int a, int b) {
  cell_t* temp = heap->cells[a];
  heap->cells[a] = heap->cells[b];
  heap->cells[b] = temp;
}

heap_t* heap_init(int size) {
  heap_t* heap = calloc(1, sizeof(heap_t));
  heap->capacity = size;
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
  if(heap->size + 1 > heap->capacity) {
    printf("Can't insert into heap (index out of bounds)\n");
    exit(1);
  }

  heap->cells[heap->size++] = cell;

  size_t i = heap->size - 1;
  while(i > 0 && heap->cells[heap_parent(i)]->cost > heap->cells[i]->cost) {
    heap_swap(heap, heap_parent(i), i);
    i = heap_parent(i);
  }

}

cell_t* heap_pop(heap_t* heap) {
  cell_t* cell = heap->cells[0];
  heap->cells[0] = NULL;
  heap_heapify(heap);
  heap->size--;
  return cell;
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

int distance(cell_t *a, cell_t *b) {
  return abs(b->x - a->x) + abs(b->y - a->y);
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

  heap_t* frontier = heap_init((DIM * DIM)); // worst case - we end up covering every cell
  cells[0]->cost = 0; // hack to stop start cost being included in total cost
  heap_insert(frontier, cells[0]);

  cell_t *c = NULL, *target = cells[hash((DIM - 1), (DIM - 1))];
  int n[4], dist;

  while(c != target) {
    c = heap_pop(frontier);
    
    for(size_t i = 0; i < frontier->size; i++) {
      if(distance(c, frontier->cells[i]) < (DIM / 10)) {
        neighbours(c, n);
        for(size_t j = 0; j < 4; j++) {
          if(n[j] > 0) {
            cost = frontier->cells[i]->cost + cells[n[j]]->cost;
            dist = distance(cells[n[j]], target);

            cells[n[j]]->cost = cost;
            heap_insert(frontier, cells[n[j]]);

            printf("(%i, %i) => %i/%i\n", cells[n[j]]->x, cells[n[j]]->y, cost, dist);
          }
        }
      } else {
        // remove from frontier
      }
    }

    break;
  }

  cell_t* answer = heap_min(frontier);
  printf("Lowest total risk: %i (%i, %i)\n", answer->cost, answer->x, answer->y);

  for(size_t i = 0; i < (DIM * DIM); i++) free(cells[i]);
  free(cells);
  for(size_t i = 0; i < (frontier->size - 1); i++) free(frontier->cells[i]);
  free(frontier);

  return 0;
}
