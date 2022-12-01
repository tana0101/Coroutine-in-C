#include <stdio.h>
#include "src/coroutine_int.h"
#define LOCAL_SIZE 16

int main(void)
{
    struct task_struct task[LOCAL_SIZE];
    struct rq rq;
    struct task_struct *tmp;

    rq_init(&rq);
    for (int i = 0; i < 20; i++) {
        if(i<LOCAL_SIZE){ // stack size: overflow limit
            task[i].tfd = i;
            tmp = rq_enqueue(&rq, &task[i]);
        }
        else  tmp = -1; // stack overflow
        
        if (tmp==0)
            printf("enqueue %d, return %d\n", i, tmp);
        else
            printf("stack is overflow\n");
    }

    for (int i = 0; i < 20; i++) {
        tmp = rq_dequeue(&rq);
        if (tmp!=NULL)
            printf("dequeue %d\n", tmp->tfd);
        else // stack is empty
            printf("stack is failed\n");
    }

    return 0;
}
