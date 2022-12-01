#include <errno.h>
#include <stdlib.h>
#include "coroutine_int.h"

void rq_init(struct rq *rq)
{
    // setup the ring buffer
    rq->top = 0; // stack top
    rq->mask = RINGBUFFER_SIZE - 1;
}

static inline unsigned int __ringbuffer_unused(struct rq *rq)
{
    return rq->mask + 1 - rq->top;
}

// enqueue at in
int rq_enqueue(struct rq *rq, struct task_struct *task)
{
    if (!__ringbuffer_unused(rq))
        return -EAGAIN;

    rq->r[rq->top++] = task; //top++
    
    return 0;
}

// dequeue at out
struct task_struct *rq_dequeue(struct rq *rq)
{
    struct task_struct *rev;

    if (rq->top <= 0) // is null
        return NULL;

    rev = rq->r[--rq->top]; //--top

    return rev;
}
