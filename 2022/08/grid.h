typedef struct grid {
  int dim; // dimension (assuming a square grid)
  int* values;
} grid_t;

grid_t* grid_from_file(char*);
void free_grid(grid_t*);
int c2ind(grid_t*, int, int);
int is_edge(grid_t*, int, int);
int is_out_of_bounds(grid_t*, int, int);
