#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

typedef struct l {
  int count;
  int* item;
} list_t;

list_t* list() {
  list_t* l = calloc(1, sizeof("list_t"));
  l->count = 0;
  l->item = calloc(32, sizeof(int));
  return l;
}

int pop_first(list_t* list) {
  if(list->count > 0) {
    int item = list->item[0];
    
    for(size_t i = 0; i < (list->count - 1); i++) {
      list->item[i] = list->item[i+1];
    }
    list->count--;
    
    return item;
  } else {
    return -1;
  }
}

void append(list_t* list, int item) {
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
  char* split;
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
      input = strtok(split, " ");
      while(input != NULL) {
        char item[3] = { input[0], input[1], '\0' }; // hacky way to extract number..
        v = atoi(item);
        append(monkey->items, v);
        input = strtok(NULL, " ");
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
}

int calculate_worry(int old, char opcode, int opvalue) {
  int new;
  int input = (opvalue > 0) ? opvalue : old;
  switch(opcode) {
    case '+':
      new = old + input;
      break;
    case '-': // don't think comes up
      new = old - input;
      break;
    case '*':
      new = old * input;
      break;
    case '/': // don't think comes up
      new = old / input;
      break;
  }
  
  return floor(new / 3);
}

int do_monkey_test(int item, int div) {
  return ((item % div) == 0);
}

int main(void) {
  int monkeys = 0;
  monkey_t** monkey = calloc(10, sizeof(monkey_t*));
  extract_monkeys("input.txt", monkey, &monkeys);
  
  int* inspected = calloc(monkeys, sizeof(int));
  
  int count, test;
  int item, worry;
  for(size_t r = 0; r < 20; r++) {
    for(size_t m = 0; m < monkeys; m++) {
      count = monkey[m]->items->count;
      //printf("Monkey %zu (has %d items)\n", m, count);
      for(size_t i = 0; i < count; i++) {
        item = pop_first(monkey[m]->items);
        //printf(" Monkey inspects an item with a worry level of %d.\n", item);
        if(item > 0) {
          worry = calculate_worry(item, monkey[m]->opcode, monkey[m]->opvalue);
          //printf("  Worry level is %c by %d, then / by 3 to %d\n", monkey[m]->opcode, monkey[m]->opvalue, worry);
          
          test = do_monkey_test(worry, monkey[m]->test);
          if(test) {
            // printf("  Current worry level is divisible by %d\n", monkey[m]->test);
            // printf("  Item with worry level %d is thrown to monkey %d\n", worry, monkey[m]->true);
            append(monkey[monkey[m]->true]->items, worry);
          } else {
            // printf("  Current worry level is not divisible by %d\n", monkey[m]->test);
            // printf("  Item with worry level %d is thrown to monkey %d\n", worry, monkey[m]->false);
            append(monkey[monkey[m]->false]->items, worry);
          }
          inspected[m]++;
        } else {
          continue;
        }
      }
    // printf("\n");
    }
  }
  
  for(size_t m = 0; m < monkeys; m++) {
    printf("Monkey %zu had %d interactions\n", m, inspected[m]);
  }
  printf("\n");
  
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
  
  int monkey_business = inspected[0] * inspected[1];
  printf("Monkey business = %d\n", monkey_business); // 151312
  
  return 0;
}
