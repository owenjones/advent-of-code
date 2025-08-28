#pragma once
#include <string.h>
#include "burrows.h"

typedef struct queue
{
    burrow_t *burrow;
    struct queue *next;
} queue_t;

queue_t *qinsert(queue_t *, burrow_t *);
burrow_t *qpop(queue_t **);
void qprint(queue_t *);
size_t qlength(queue_t *);
void qfree(queue_t *);