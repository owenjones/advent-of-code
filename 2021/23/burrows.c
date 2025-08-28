#include "burrows.h"

burrow_t load_burrow(char *file)
{
    FILE *fptr;
    if ((fptr = fopen(file, "r")) == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    char input[54];
    fread(input, sizeof(char), 54, fptr);
    fclose(fptr);

    burrow_t burrow = {
        {{input[31], input[45]}, {input[33], input[47]}, {input[35], input[49]}, {input[37], input[51]}},
        "...........",
        0};

    return burrow;
}

void display_burrow(burrow_t *burrow)
{
    printf("#############\n#");
    for (size_t i = 0; i < 11; i++)
        printf("%c", burrow->hallway[i]);
    printf("#\n###%c#%c#%c#%c###\n  #%c#%c#%c#%c#\n  #########\n\n",
           burrow->rooms[0][0], burrow->rooms[1][0], burrow->rooms[2][0], burrow->rooms[3][0],
           burrow->rooms[0][1], burrow->rooms[1][1], burrow->rooms[2][1], burrow->rooms[3][1]);
}

uint8_t is_organised(burrow_t *burrow)
{
    return (
        burrow->rooms[0][0] == 'A' &&
        burrow->rooms[0][1] == 'A' &&
        burrow->rooms[1][0] == 'B' &&
        burrow->rooms[1][1] == 'B' &&
        burrow->rooms[2][0] == 'C' &&
        burrow->rooms[2][1] == 'C' &&
        burrow->rooms[3][0] == 'D' &&
        burrow->rooms[3][1] == 'D');
}

uint8_t target_room(char amphipod)
{
    switch (amphipod)
    {
    case 'A':
        return 0;
    case 'B':
        return 1;
    case 'C':
        return 2;
    case 'D':
        return 3;
    default:
        return 255;
    }
}

uint8_t hallway2room(uint8_t idx)
{
    switch (idx)
    {
    case 2:
        return 0;
    case 4:
        return 1;
    case 6:
        return 2;
    case 8:
        return 3;
    default:
        return 255;
    }
}

uint8_t room2hallway(uint8_t idx)
{
    switch (idx)
    {
    case 0:
        return 2;
    case 1:
        return 4;
    case 2:
        return 6;
    case 3:
        return 8;
    default:
        return 255;
    }
}

uint8_t can_move_from_hallway(burrow_t *burrow, uint8_t hallway, uint8_t room)
{
    // can we move from the current hallway square to the room square
    for (size_t i = hallway + 1; i <= room2hallway(room); i++)
    {
        if (burrow->hallway[i] != '.')
            return 0;
    }

    return 1;
}

uint8_t can_move_to_hallway(burrow_t *burrow, uint8_t room, uint8_t hallway)
{
}

uint32_t movement_cost(char amphipod, uint8_t moves)
{
    uint32_t multiplier = 0;
    switch (amphipod)
    {
    case 'A':
        multiplier = 1;
    case 'B':
        multiplier = 10;
    case 'C':
        multiplier = 100;
    case 'D':
        multiplier = 1000;
    default:
        multiplier = 0;
    }

    return multiplier * moves;
}