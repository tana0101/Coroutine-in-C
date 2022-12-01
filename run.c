#include <stdio.h>
#include "src/coroutine_int.h"

int main(void)
{
    struct task_struct task[30];
    struct rq rq;
    struct task_struct *tmp;

    rq_init(&rq);
    for (int i = 0; i < 20; i++) {
        task[i].tfd = i;
        tmp = rq_enqueue(&rq, &task[i]);
        if (tmp==0)
            printf("enqueue %d, return %d\n", i, tmp);
        else
            printf("stack is overflow\n");
    }

    for (int i = 0; i < 20; i++) {
        tmp = rq_dequeue(&rq);
        if (tmp!=NULL)
            printf("dequeue %d\n", tmp->tfd);
        else
            printf("stack is failed\n");
    }

    return 0;
}
