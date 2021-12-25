#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rule {
  char left;
  char right;
  char element;
} rule_t;

char* fold(char* template, rule_t** rules, int nrules) {
  char* new = (char*) calloc(strlen(template) * 2, sizeof(char));
  char* insert;
  
  new[0] = template[0];
  
  for(size_t i = 0; i < strlen(template); i++) {
    insert = (char*) calloc(3, sizeof(char));

    for(size_t r = 0; r < nrules; r++) {
      if(template[i] == rules[r]->left && template[i+1] == rules[r]->right) {
        sprintf(insert, "%c%c", rules[r]->element, rules[r]->right);
        strcat(new, insert);
        break;
      }
    }
    
    if(!insert) {
      sprintf(insert, "%c", template[i+1]);
      strcat(new, insert);
    }
    
    free(insert);
  }
  
  free(template);
  return new;
}

int main(void) {
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  char* template = (char*) calloc(20, sizeof(char));
  fscanf(fptr, "%s\n", template);
  
  rule_t **rules = (rule_t**) calloc(100, sizeof(rule_t*));
  char left, right, element;
  int nrules = 0;
  while(fscanf(fptr, "%1c%1c -> %1c\n", &left, &right, &element) > 0) {
    rule_t *rule = (rule_t*) calloc(1, sizeof(rule_t));
    rule->left = left;
    rule->right = right;
    rule->element = element;
    rules[nrules++] = rule;
  }

  fclose(fptr);

  for(size_t i = 0; i < 40; i++) {
    printf("%zu...\n", i);
    template = fold(template, rules, nrules);
  }

  int* counts = (int*) calloc(26, sizeof(int));
  for(size_t i = 0; i < strlen(template); i++) {
    counts[(template[i] - 65)]++;
  }

  size_t i = 0;
  int temp;
  while(i < 25) {
    if(counts[i] < counts[i+1]) {
      temp = counts[i];
      counts[i] = counts[i+1];
      counts[i+1] = temp;
      i = 0;
    } else {
      i++;
    }
  }

  i = 0;
  while(counts[i] > 0) i++;
  
  int answer = counts[0] - counts[i-1]; // 3143
  printf("Frequency of most common element minus frequency of least common: %i\n", answer);
  return 0;
}
