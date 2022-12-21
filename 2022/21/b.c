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

monkey_t* get_monkey(monkey_t** monkies, char id[4]) {
  return monkies[monkey_hash(id)];
}

monkey_t* make_or_get_monkey(monkey_t** monkies, char id[4]) {
  monkey_t* monkey = get_monkey(monkies, id);
  
  if(monkey == NULL) {
    uint32_t hash = monkey_hash(id);
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

monkey_t** extract_monkies(char* file) {
  FILE* fptr;
  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  monkey_t** monkies = calloc(421075226, sizeof(monkey_t*));

  char id[4], left_id[4], right_id[4], operation;
  int64_t value;
  monkey_t* monkey;

  char* input = NULL;
  size_t bufsize = 0;
  while(getline(&input, &bufsize, fptr) != -1) {
    if(sscanf(input, "%4c: %lld\n", id, &value) == 2) {
      monkey = make_or_get_monkey(monkies, id);
      if(strcmp(id, "humn") == 0) {
        monkey->type = NOT_A_MONKEY;
        monkey->branch_contains_human = 1;
      } else {
        monkey->type = VALUE_MONKEY;
        monkey->value = value;
      }
    } else if(sscanf(input, "%4c: %4c %1c %4c\n", id, left_id, &operation, right_id) == 4) {
      monkey = make_or_get_monkey(monkies, id);
      monkey->left = make_or_get_monkey(monkies, left_id);
      monkey->right = make_or_get_monkey(monkies, right_id);
      
      if(strcmp(id, "root") == 0) {
        monkey->type = ROOT_MONKEY;
        monkey->operation = '=';
      } else {
        monkey->type = OPERATION_MONKEY;
        monkey->operation = operation;
      }
    }
  }
  fclose(fptr);
  free(input);
  
  return monkies;
}

void free_monkies(monkey_t** monkies) {
  for(size_t m = 0; m < 421075226; m++) {
    if(monkies[m] != NULL) free(monkies[m]);
  }
  free(monkies);
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

void find_human_value(monkey_t* monkey, int64_t input) {
  if(monkey->type == NOT_A_MONKEY) {
    monkey->value = input;
    return;
  }
  
  uint8_t in_left = monkey->left->branch_contains_human;
  monkey_t* branch = (in_left) ? monkey->right : monkey->left;
  int64_t solution = solve_branch(branch);
  
  if(monkey->type != ROOT_MONKEY) {
    solution = do_invert_operation(
      monkey->operation,
      input,
      solution,
      in_left
    );
  }
  
  branch = (in_left) ? monkey->left : monkey->right;
  find_human_value(branch, solution);
}

int main(void) {
  monkey_t** monkies = extract_monkies("input.txt");
  monkey_t* root = get_monkey(monkies, "root");
  unmask_humans(root);
  find_human_value(root, 0);

  int64_t shout = get_monkey(monkies, "humn")->value;
  printf("Number human has to shout = %lld\n", shout);
  // 3715799488132

  free_monkies(monkies);
  return 0;
}
