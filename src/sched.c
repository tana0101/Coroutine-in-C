#define _GNU_SOURCE
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sched.h>

#include "context.h"
#include "coroutine.h"
#include "coroutine_int.h"

/* LIFO scheduler */

static inline int lifo_schedule(struct cr *cr, job_t func, void *args)
{
    struct task_struct *new_task;

    new_task = calloc(1, sizeof(struct task_struct));
    if (!new_task)
        return -ENOMEM;
    if (rq_enqueue(&cr->rq, new_task) < 0) {
        free(new_task);
        return -ENOMEM;
    }

    new_task->cr = cr;
    new_task->tfd = cr->size++;
    new_task->job = func;
    new_task->args = args;
    new_task->context.label = NULL;
    new_task->context.wait_yield = 1;
    new_task->context.blocked = 1;

    return new_task->tfd;
}

static inline struct task_struct *lifo_pick_next_task(struct cr *cr)
{
    return rq_dequeue(&cr->rq);
}

static inline int lifo_put_prev_task(struct cr *cr, struct task_struct *prev)
{
    return rq_enqueue(&cr->rq, prev);
}

void sched_init(struct cr *cr)
{
    switch (cr->flags) {
    case CR_LIFO:
        rq_init(&cr->rq);
        cr->schedule = lifo_schedule;
        cr->pick_next_task = lifo_pick_next_task;
        cr->put_prev_task = lifo_put_prev_task;
        return;
    }
}
