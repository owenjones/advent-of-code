#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

uint32_t roll_dice(uint32_t rolls) {
  return (rolls % 100) + 1;
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  uint8_t i, s, position[2];
  while(fscanf(fptr, "Player %hhu starting position: %hhu\n", &i, &s) == 2)
    position[(i - 1)] = s;
  fclose(fptr);

  uint8_t rolls[9][2] = {
    // given all possible roll combinations (27), how many times will we see a score:
    { 3, 1 }, { 4, 3 }, { 5, 6 }, { 6, 7 }, { 7, 6 }, { 8, 3 }, { 9, 1 }
    };

  uint64_t scores[2] = {0, 0};

  

  uint64_t score = (scores[0] > scores[1]) ? scores[0] : scores[1];
  printf("Score of losing player multipled by number of rolls: %llu\n", score); // 512442
  return 0;
}
