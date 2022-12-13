typedef struct point {
  int x, y;
} point_t;

typedef struct map {
  int w, h;
  int* heights;
  point_t* start;
  point_t* end;
} map_t;

typedef struct node {
  point_t* point;
  int h, g, f;
} node_t;

typedef struct nodelist {
  int n;
  node_t** nodes;
} list_t;

// Map
map_t* new_map();
map_t* load_map(char*);
void free_map(map_t*);

// Points
point_t* new_point();
point_t* new_point_at(int, int);
int can_climb_up(map_t*, point_t*, point_t*);
int manhattan_distance(point_t*, point_t*);
int is_same(point_t*, point_t*);
int is_valid(map_t*, point_t*);
int height_at(map_t*, point_t*);

// Nodes
node_t* new_node_at(int, int);
void free_node(node_t*);
node_t* step_from(node_t*, int);
void update_node_heuristics(map_t*, node_t*, node_t*);

// Lists
list_t* new_list();
void free_list(list_t*);
node_t* get_head_node(list_t*);
node_t* remove_node_at(list_t*, int);
void append_node(list_t*, node_t*);
int find_lowest_node(list_t*);
int lower_exists_in(list_t*, node_t*);

// Pathfinding
int calculate_risk(map_t*);
