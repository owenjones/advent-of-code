#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

typedef struct l {
  int count;
  uint64_t* item;
} list_t;

list_t* list() {
  list_t* l = calloc(1, sizeof(list_t));
  l->count = 0;
  l->item = calloc(32, sizeof(uint64_t));
  return l;
}

uint64_t pop_first(list_t* list) {
  if(list->count > 0) {
    uint64_t item = list->item[0];

    for(size_t i = 0; i < (list->count - 1); i++) {
      list->item[i] = list->item[i+1];
    }
    list->count--;

    return item;
  } else {
    return 0;
  }
}

void append(list_t* list, uint64_t item) {
  list->item[list->count++] = item;
}

typedef struct monkey {
  int n; // "Monkey <id>:"
  list_t* items; // "Starting items: <list>"
  char opcode; // Operation: new = old <opcode> <opvalue>
  int opvalue; // >= 0: use value with opcode; <= 0 use 'old' value with opcode
  int test; // "divisible by <test>"
  int true; // "If true: throw to monkey <true>"
  int false; // "If false: throw to monkey <false>"
} monkey_t;

void extract_monkeys(char* file, monkey_t** monkeys, int* nmonkey) {
  // horrible hacky way to extract monkey data from input file into usable structs

  FILE* fptr;
  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  monkey_t* monkey;
  int v;
  char *split, *tok;
  char opcode;
  int opvalue;

  char* input = NULL;
  size_t bufsize = 0;
  while(getline(&input, &bufsize, fptr) != -1) {
    if(sscanf(input, "Monkey %d:\n", &v) == 1) {
      monkey = calloc(1, sizeof(monkey_t));
      monkey->items = list();
      monkeys[v] = monkey;
      (*nmonkey)++;
    } else if((split = strstr(input, "items: ")) != NULL) {
      split += 7; // skip "items: " in string
      tok = strtok(split, " ");
      while(tok != NULL) {
        char item[3] = { tok[0], tok[1], '\0' }; // hacky way to extract number..
        v = atoi(item);
        append(monkey->items, v);
        tok = strtok(NULL, " ");
      }
    } else if(sscanf(input, " Operation: new = old %c %d", &opcode, &opvalue) == 2) {
      monkey->opcode = opcode;
      monkey->opvalue = opvalue;
    } else if(sscanf(input, " Operation: new = old %c old", &opcode) == 1) {
      monkey->opcode = opcode;
      monkey->opvalue = -1;
    } else if(sscanf(input, " Test: divisible by %d", &v) == 1) {
      monkey->test = v;
    } else if(sscanf(input, "  If true: throw to monkey %d", &v) == 1) {
      monkey->true = v;
    } else if(sscanf(input, "  If false: throw to monkey %d", &v) == 1) {
      monkey->false = v;
    }
  }
  fclose(fptr);
  free(input);
  free(split);
  free(tok);
}

int calculate_lcm(monkey_t** monkey, int monkeys) {
  int lcm = 1;
  for(size_t i = 0; i < monkeys; i++) {
    // monkey test divisors are all prime, so LCM is product of them all
    lcm *= monkey[i]->test;
  }
  return lcm;
}

uint64_t calculate_worry(uint64_t old, char opcode, int opvalue, int lcm) {
  uint64_t new;
  int input = (opvalue > 0) ? opvalue : old;
  switch(opcode) {
    case '+':
      new = old + input;
      break;
    case '*':
      new = old * input;
      break;
  }

  return new % lcm;
}

int passes_monkey_test(uint64_t item, int div) {
  return ((item % div) == 0);
}

void free_monkeys(monkey_t** monkey, int monkeys) {
  for(size_t m = 0; m < monkeys; m++) {
    free(monkey[m]->items->item);
    free(monkey[m]->items);
    free(monkey[m]);
  }
  free(monkey);
}

uint64_t monkey_business(monkey_t** monkey, int monkeys, int rounds) {
  int lcm = calculate_lcm(monkey, monkeys);
  uint64_t* inspected = calloc(monkeys, sizeof(uint64_t));

  int count;
  uint64_t item, worry;

  for(size_t r = 0; r < rounds; r++) {
    for(size_t m = 0; m < monkeys; m++) {
      count = monkey[m]->items->count;
      for(size_t i = 0; i < count; i++) {
        item = pop_first(monkey[m]->items);
        worry = calculate_worry(item, monkey[m]->opcode, monkey[m]->opvalue, lcm);
        if(passes_monkey_test(worry, monkey[m]->test)) {
          append(monkey[monkey[m]->true]->items, worry);
        } else {
          append(monkey[monkey[m]->false]->items, worry);
        }
        inspected[m]++;
      }
    }
  }

  // for(size_t m = 0; m < monkeys; m++) {
  //   printf("Monkey %zu had %llu interactions\n", m, inspected[m]);
  // }
  // printf("\n");

  // Sort descending
  for(size_t i = 0; i < (monkeys - 1); i++) {
    for(size_t j = 0; j < (monkeys - i - 1); j++) {
      if(inspected[j+1] > inspected[j]) {
        int temp = inspected[j];
        inspected[j] = inspected[j+1];
        inspected[j+1] = temp;
      }
    }
  }

  uint64_t business = inspected[0] * inspected[1];
  free(inspected);
  return business;
}

int main(void) {
  int monkeys = 0;
  monkey_t** monkey = calloc(10, sizeof(monkey_t*));
  extract_monkeys("input.txt", monkey, &monkeys);

  uint64_t business = monkey_business(monkey, monkeys, 10000);
  printf("Monkey business = %llu\n", business); // 51382025916

  free_monkeys(monkey, monkeys);
  return 0;
}
