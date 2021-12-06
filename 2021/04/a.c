#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int value;
  int marked;
} number;

typedef struct {
  number numbers[25];
} board;

board* init_board(int* t) {
  board* b = calloc(1, sizeof(board));
  for(int i = 0; i < 25; i++) {
    b->numbers[i].value = t[i];
    b->numbers[i].marked = 0;
  }
  
  return b;
}

void mark_board(board* board, int number) {
  for(int i = 0; i < 25; i++) {
    if(board->numbers[i].value == number) {
      board->numbers[i].marked = 1;
      return;
    }
  }
}

int test_board(board* board) {
  int x = 0, y = 0, d = 0;
  for(int i = 0; i < 5; i++) {
    d = i * 5;
    x = (board->numbers[d].marked 
      + board->numbers[d+1].marked 
      + board->numbers[d+2].marked 
      + board->numbers[d+3].marked 
      + board->numbers[d+4].marked);
      
    y = (board->numbers[i].marked 
      + board->numbers[i+5].marked 
      + board->numbers[i+10].marked 
      + board->numbers[i+15].marked 
      + board->numbers[i+20].marked);
      
    if(x == 5 || y == 5) return 1;
  }
  
  return 0;
}

int get_board_score(board* board, int last) {
  int total = 0;
  for(int i = 0; i < 25; i++) {
    if(board->numbers[i].marked == 0) {
      total += board->numbers[i].value;
    }
  }
  
  return total * last;
}

int main(void) {
  char board_input[] = "boards.txt";
  FILE* fptr;

  if((fptr = fopen(board_input, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  board* boards[100];
  int t[25];
  for(int i = 0; i < 100; i++) {
    for(int n = 0; n < 25; n++) {
      fscanf(fptr, "%i", &t[n]);
    }
    
    boards[i] = init_board(t);
  }

  fclose(fptr);
  
  char number_input[] = "numbers.txt";
  
  if((fptr = fopen(number_input, "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  
  int n, answer = 0;
  while(!answer) {
    fscanf(fptr, "%i,", &n);
    for(int i = 0; i < 100; i++) {
      mark_board(boards[i], n);
      if(test_board(boards[i])) answer = get_board_score(boards[i], n);
    }
  }
  
  fclose(fptr);

  printf("First winning board score = %i\n", answer);
  return 0;
}
