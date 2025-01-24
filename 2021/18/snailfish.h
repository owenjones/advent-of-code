#include <stdint.h>

typedef struct node
{
  int8_t value;
  uint8_t depth;
  struct node *parent;
  struct node *left;
  struct node *right;
  uint8_t ud;
  uint8_t lr;
} node_t;

void debug(const char *format, ...);
void to_string(node_t *node);
void _to_string(node_t *node);

node_t *init_node(node_t *parent, uint8_t depth);
void free_tree(node_t *);
node_t *parse_line(char *line);

void reduce(node_t *root);

uint8_t explode(node_t *node);
uint8_t split(node_t *node);

void increase_depth(node_t *node);

node_t *add_numbers(node_t *left, node_t *right);
uint32_t calculate_magnitude(node_t *node);
