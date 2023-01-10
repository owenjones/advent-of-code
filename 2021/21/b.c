#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct pair
{
  uint64_t value[2];
} pair_t;

pair_t count_wins(uint8_t position_1, int8_t total_1, uint8_t position_2, int8_t total_2)
{
  static uint8_t rolls[9][2] = {
      // given all possible roll combinations (27), how many times will we see a score:
      // { score, times }
      {3, 1},
      {4, 3},
      {5, 6},
      {6, 7},
      {7, 6},
      {8, 3},
      {9, 1}};

  pair_t pair;
  pair.value[0] = 0;
  pair.value[1] = 0;

  if (total_2 <= 0)
  {
    pair.value[1] = 1;
    return pair;
  }

  for (uint8_t i = 0; i < 7; i++)
  {
    uint8_t move = (position_1 + rolls[i][0]) % 10;
    pair_t count = count_wins(position_2, total_2, move, (total_1 - move - 1));

    pair.value[0] += count.value[1] * rolls[i][1];
    pair.value[1] += count.value[0] * rolls[i][1];
  }

  return pair;
}

int main(void)
{
  FILE *fptr;
  if ((fptr = fopen("input.txt", "r")) == NULL)
  {
    printf("Error opening file\n");
    exit(1);
  }

  uint8_t i, s, position[2];
  while (fscanf(fptr, "Player %hhu starting position: %hhu\n", &i, &s) == 2)
    position[(i - 1)] = (s - 1);
  fclose(fptr);

  pair_t scores = count_wins(position[0], 21, position[1], 21);
  uint64_t score = (scores.value[0] > scores.value[1]) ? scores.value[0] : scores.value[1];
  printf("Most universe wins: %llu\n", score); //
  return 0;
}
