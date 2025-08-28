#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    char rooms[4][2];
    char hallway[11];
    uint32_t cost;
} burrow_t;

burrow_t load_burrow(char *);
void display_burrow(burrow_t *);
uint8_t is_organised(burrow_t *);

uint8_t target_room(char);
uint8_t hallway2room(uint8_t);
uint8_t room2hallway(uint8_t);
uint8_t can_move_from_hallway(burrow_t *, uint8_t, uint8_t);
uint32_t movement_cost(char, uint8_t);