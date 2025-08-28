#include "queue.h"

queue_t *qinsert(queue_t *queue, burrow_t *burrow)
{
    queue_t *head = queue;
    if (head == NULL)
    {
        queue_t *e = malloc(sizeof(queue_t));
        e->burrow = burrow;
        e->next = NULL;
        return e;
    }

    if (burrow->cost <= head->burrow->cost)
    {
        queue_t *e = malloc(sizeof(queue_t));
        e->burrow = burrow;
        e->next = head;
        return e;
    }

    while (head->next && head->next->burrow->cost <= burrow->cost)
        head = head->next;

    queue_t *e = malloc(sizeof(queue_t));
    e->burrow = burrow;
    e->next = head->next;
    head->next = e;

    return queue;
}

burrow_t *qpop(queue_t **queue)
{
    if ((*queue) == NULL)
        return NULL;

    burrow_t *head = (*queue)->burrow;
    queue_t *next = (*queue)->next;
    free((*queue));
    (*queue) = next;
    return head;
}

void qprint(queue_t *queue)
{
    while (queue)
    {
        printf("%i, ", queue->burrow->cost);
        queue = queue->next;
    }

    printf("\n");
}

size_t qlength(queue_t *queue)
{
    size_t i = 0;
    while (queue != NULL)
    {
        i++;
        queue = queue->next;
    }

    return i;
}

void qfree(queue_t *queue)
{
    if (queue)
    {
        queue_t *next = queue->next;
        free(queue->burrow);
        free(queue);
        qfree(next);
    }
}