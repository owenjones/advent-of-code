//#define DEBUG
#define INPUT "input.txt"
#define STACKS 9

typedef struct crate {
  char id;
  struct crate* next;
} crate_t;

crate_t** init_stacks();
void fill_stacks(crate_t**, FILE*);
void print_stacks(crate_t**);
void free_stacks(crate_t**);
void move_crates_individually(crate_t**, int, int, int);
void move_crates_grouped(crate_t**, int, int, int);
void get_top_crates(crate_t**);
