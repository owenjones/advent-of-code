#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct fish {
  int8_t value;
  struct fish* next;
} fish_t;

int count(fish_t* head) {
  fish_t* fish = head;
  int count = 0;
  while(fish->next != NULL) {
    count++;
    fish = fish->next;
  }
  
  return count;
}

void disp_pop(fish_t* head) {
  while(head != NULL) {
    printf("%i,", head->value);
    head = head->next;
  }
  printf("\n");
}

int main(void) {
  char file[] = "test_input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  fish_t* head = (fish_t*) calloc(1, sizeof(fish_t));
  fish_t* fish = head;
  int n;
  while(fscanf(fptr, "%i,", &n) > 0) {
    fish->value = n;
    fish->next = (fish_t*) calloc(1, sizeof(fish_t));
    fish = fish->next;
  } // CREATING AN OFF-BY-ONE HERE WITH FINAL POINTER

  fclose(fptr);

  printf("Initial size = %i\n", count(head));
  disp_pop(head);

  for(int d = 0; d < 18; d++) {
    printf("Day %i = %i\n", d, count(head));
    fish_t* fish = head;
    int add = 0;
    while(fish->next != NULL) {
      fish->value--;
      
      if(fish->value < 0) {
        fish->value = 6;
        add++;
      }
      
      fish = fish->next;
    }
    
    for(int i = 0; i < add; i++) {
      fish->next = (fish_t*) calloc(1, sizeof(fish_t));
      fish = fish->next;
      fish->value = 8;
    }
  }
  
  int c = count(head);
  printf("population after 80 days = %i\n", c);

  return 0;
}
