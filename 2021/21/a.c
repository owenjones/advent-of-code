#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

uint32_t roll_dice(uint32_t rolls)
{
  return (rolls % 100) + 1;
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

  uint32_t rolls = 0, d = 1, move, pos;
  uint32_t scores[2] = {0, 0};

  uint8_t p = 0;
  while (1)
  {
    move = roll_dice(rolls++);
    move += roll_dice(rolls++);
    move += roll_dice(rolls++);

    position[p] = ((position[p] + move) % 10);
    scores[p] += position[p] + 1;

    if (scores[p] >= 1000)
      break;

    p = (p + 1) % 2;
  }

  uint8_t loser = (p + 1) % 2;
  uint32_t score = scores[loser] * rolls;
  printf("Score of losing player multipled by number of rolls: %i\n", score); // 512442
  return 0;
}
