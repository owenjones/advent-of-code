#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

enum monkey_type { 
  UNKNOWN_MONKEY,
  VALUE_MONKEY,
  OPERATION_MONKEY,
  ROOT_MONKEY,
  NOT_A_MONKEY
};

typedef struct monkey {
  enum monkey_type type;
  int64_t value;
  char operation;
  struct monkey *left, *right;
  uint8_t branch_contains_human;
} monkey_t;

uint32_t monkey_hash(char id[4]) {
  return ((id[0] - 97) << 24) + ((id[1] - 97) << 16) + ((id[2] - 97) << 8) + (id[3] - 97);
}

monkey_t* make_or_get_monkey(monkey_t** monkies, char id[4]) {
  uint32_t hash = monkey_hash(id);
  monkey_t* monkey = monkies[hash];
  
  if(monkey == NULL) {
    monkey = calloc(1, sizeof(monkey_t));
    monkey->type = UNKNOWN_MONKEY;
    monkey->value = 0;
    monkey->operation = '?';
    monkey->left = NULL;
    monkey->right = NULL;
    monkey->branch_contains_human = 0;
    monkies[hash] = monkey;
  }
  
  return monkey;
}

int64_t do_operation(char op, int64_t a, int64_t b) {
  switch(op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return a / b;
    case '=': return -1;
  }
  return -1;
}

int64_t do_invert_operation(char op, int64_t a, int64_t b, int is_left) {
  switch(op) {
    case '+': 
      return a - b;
    
    case '-':
      if(is_left) return a + b;
      return -(a - b);

    case '*': 
      return a / b;
    
    case '/':
      if(is_left) return a * b;
      return b / a;
      
    case '=': return -1;
  }
  return -1;
}

uint8_t unmask_humans(monkey_t* monkey) {
  uint8_t contains;
  switch(monkey->type) {
    case UNKNOWN_MONKEY:
      printf("Encountered an unknown monkey - this shouldn't happen!\n");
      exit(1);
      
    case VALUE_MONKEY:
      return 0;
      
    case OPERATION_MONKEY:
      contains = (unmask_humans(monkey->left) || unmask_humans(monkey->right));
      monkey->branch_contains_human = contains;
      return contains;
      
    case ROOT_MONKEY:
      contains = (unmask_humans(monkey->left) || unmask_humans(monkey->right));
      monkey->branch_contains_human = contains;
      return contains;
      
    case NOT_A_MONKEY:
      return 1;
      
  }
  
}

int64_t solve_branch(monkey_t* monkey) {
  if(monkey->branch_contains_human) {
    printf("Can't solve - this branch has the human (unknown value) in it");
    exit(1);
  }
  
  int64_t solution;
  switch(monkey->type) {
    case UNKNOWN_MONKEY:
      printf("Encountered an unknown monkey - this shouldn't happen!\n");
      exit(1);
      
    case VALUE_MONKEY:
      return monkey->value;
      
    case OPERATION_MONKEY:
      solution = do_operation(
        monkey->operation,
        solve_branch(monkey->left),
        solve_branch(monkey->right)
      );
      
      monkey->type = VALUE_MONKEY;
      monkey->value = solution;
      return solution;
      
    default:
      return 0;
  }
}

void find_human_value(monkey_t* monkey, int64_t target) {
  if(monkey->type == NOT_A_MONKEY) {
    monkey->value = target;
    return;
  }
  
  int64_t left, right, solution;
  
  if(monkey->left->branch_contains_human) {
    right = solve_branch(monkey->right);
    solution = do_invert_operation(
      monkey->operation,
      target,
      right,
      1
    );
    
    find_human_value(monkey->left, solution);
    
  } else if(monkey->right->branch_contains_human) {
    left = solve_branch(monkey->left);
    solution = do_invert_operation(
      monkey->operation,
      target,
      left,
      0
    );
    
    find_human_value(monkey->right, solution);
  }
  
}

void start_search(monkey_t* root) {
  if(root->left->branch_contains_human) {
    int64_t solution = solve_branch(root->right);
    printf("Human is in left branch, right branch solution is %lld\n", solution);
    find_human_value(root->left, solution);
  } else if(root->right->branch_contains_human) {
    int64_t solution = solve_branch(root->left);
    printf("Human is in right branch, left branch solution is %lld\n", solution);
    find_human_value(root->right, solution);
  }
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  monkey_t** monkies = calloc(421075226, sizeof(monkey_t*));

  char id[4], left_id[4], right_id[4], operation;
  int64_t value;
  monkey_t *monkey, *left, *right;

  char* input = NULL;
  size_t bufsize = 0;
  while(getline(&input, &bufsize, fptr) != -1) {
    if(sscanf(input, "%4c: %lld\n", id, &value) == 2) {
      if(strcmp(id, "humn") == 0) {
        monkey = make_or_get_monkey(monkies, id);
        monkey->type = NOT_A_MONKEY;
        monkey->branch_contains_human = 1;
      } else {
        monkey = make_or_get_monkey(monkies, id);
        monkey->type = VALUE_MONKEY;
        monkey->value = value;
      }
      
    } else if(sscanf(input, "%4c: %4c %1c %4c\n", id, left_id, &operation, right_id) == 4) {
      monkey = make_or_get_monkey(monkies, id);
      left = make_or_get_monkey(monkies, left_id);
      right = make_or_get_monkey(monkies, right_id);
      
      if(strcmp(id, "root") == 0) {
        monkey->type = ROOT_MONKEY;
        monkey->operation = '=';
      } else {
        monkey->type = OPERATION_MONKEY;
        monkey->operation = operation;
      }
      
      monkey->left = left;
      monkey->right = right;
    }
  }
  fclose(fptr);
  free(input);

  monkey_t* root = make_or_get_monkey(monkies, "root");
  unmask_humans(root);
  start_search(root);

  int64_t shout = make_or_get_monkey(monkies, "humn")->value;
  printf("Number human has to shout = %lld\n", shout); 

  for(size_t m = 0; m < 421075226; m++) {
    if(monkies[m] != NULL) free(monkies[m]);
  }

  return 0;
}
