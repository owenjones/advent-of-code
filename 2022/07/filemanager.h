enum type { directory, file };

typedef struct node {
  char id[128];
  enum type type;
  uint32_t size;
  struct node* parent;
  int children;
  struct node** child;
} node_t;

void add_child(node_t*, node_t*);
int find_child(node_t*, char*);
void update_sizes(node_t*);
node_t* create_node(char*, enum type, uint32_t, node_t*);
void build_tree(FILE*, node_t*);
void free_nodes(node_t*);
