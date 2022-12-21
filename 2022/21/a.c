#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

struct operation {
  char op;
  struct monkey *left, *right;
};

typedef struct monkey {
  int64_t value;
  uint8_t yelled;
  int listening;
  struct monkey** listeners;
  struct operation operation;
} monkey_t;

uint32_t monkey_hash(char id[4]) {
  return ((id[0] - 97) << 24) + ((id[1] - 97) << 16) + ((id[2] - 97) << 8) + (id[3] - 97);
}

monkey_t* make_or_get_monkey(monkey_t** monkies, char id[4]) {
  uint32_t hash = monkey_hash(id);
  monkey_t* monkey = monkies[hash];
  
  if(monkey == NULL) {
    monkey = calloc(1, sizeof(monkey_t));
    monkey->yelled = 0;
    monkey->value = 0;
    monkey->listening = 0;
    monkey->listeners = calloc(100, sizeof(monkey_t*));
    monkies[hash] = monkey;
  }
  
  return monkey;
}

int64_t do_operation(char op, int64_t a, int64_t b) {
  switch(op) {
    case '+':
      return a + b;
    case '-':
      return a - b;
    case '*':
      return a * b;
    case '/':
      return a / b;
  }
  return 0;
}

void add_listener(monkey_t* target, monkey_t* listener) {
  target->listeners[target->listening++] = listener;
}

void yell(monkey_t* monkey) {
  monkey->yelled = 1;
  if(monkey->listening > 0) {
    for(size_t m = 0; m < monkey->listening; m++) {
      if(monkey->listeners[m]->operation.left->yelled && monkey->listeners[m]->operation.right->yelled) {
        monkey->listeners[m]->value = do_operation(monkey->listeners[m]->operation.op, monkey->listeners[m]->operation.left->value, monkey->listeners[m]->operation.right->value);
        yell(monkey->listeners[m]);
      }
    }
  monkey->listening = 0;
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
      // if monkey has a value ->
      // make/get the monkey, save value
      // then 'yell' so if monkey has listeners they can calculate their operation
      // if values are now available
      monkey = make_or_get_monkey(monkies, id);
      monkey->value = value;
      yell(monkey);
      
    } else if(sscanf(input, "%4c: %4c %1c %4c\n", id, left_id, &operation, right_id) == 4) {
      // if monkey has an operation ->
      // make/get the monkey
      // if both sides have values availanble calculate the operation output and save;
      // if one/both sides are missing save the operation and add current monkey as
      // a listener, to be handled when a value becomes available
      monkey = make_or_get_monkey(monkies, id);
      left = make_or_get_monkey(monkies, left_id);
      right = make_or_get_monkey(monkies, right_id);
      
      if(left->yelled && right->yelled) {
        // both monkies have values so we can do the operation straight away and yell
        monkey->value = do_operation(operation, left->value, right->value);
        yell(monkey);
      } else {
        // otherwise: save operation info and add this monkey as a listener to
        // whichever value we're waiting on
        monkey->operation.op = operation;
        monkey->operation.left = left;
        monkey->operation.right = right;
        
        if(!left->yelled) {
          add_listener(left, monkey);
        }
        
        if(!right->yelled) {
          add_listener(right, monkey);
        }
      }
      
    }
  }
  fclose(fptr);

  int64_t root = make_or_get_monkey(monkies, "root")->value;
  printf("Value of root monkey = %lld\n", root); // 331319379445180

  for(size_t m = 0; m < 421075226; m++) {
    if(monkies[m] != NULL) {
      free(monkies[m]->listeners);
      free(monkies[m]);
    }
  }

  return 0;
}
