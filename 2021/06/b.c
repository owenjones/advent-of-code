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
  while(fish != NULL) {
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
  char file[] = "input.txt";
  FILE* fptr;

  if((fptr = fopen(file, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  fish_t* head = NULL;
  fish_t* fish;
  int n;
  while(fscanf(fptr, "%i,", &n) > 0) {
    if(!head) {
      fish = (fish_t*) malloc(sizeof(fish_t));
      head = fish;
    } else {
      fish->next = (fish_t*) malloc(sizeof(fish_t));
      fish = fish->next;
    }
    fish->value = n;
  }

  fclose(fptr);

  printf("Initial size = %i\n", count(head));
  // disp_pop(head);

  fish_t* last;
  for(int d = 0; d < 256; d++) {
    printf("Day %i\n", d);
    fish = head;
    int add = 0;
    while(fish != NULL) {
      fish->value--;
      
      if(fish->value < 0) {
        fish->value = 6;
        add++;
      }
      
      last = fish;
      fish = fish->next;
    }
    
    for(int i = 0; i < add; i++) {
      last->next = (fish_t*) malloc(sizeof(fish_t));
      last = last->next;
      last->value = 8;
    }
  }
  
  int c = count(head);
  printf("population after 256 days = %i\n", c);

  return 0;
}
