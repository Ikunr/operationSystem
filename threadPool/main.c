#include "threadPool.h"
#include <stdio.h>
#include <unistd.h>

void * printData(void *arg)
{
    int val = *(int *)arg;
    printf("hello world, val:%d\n", val);
    return NULL;
}

int main()
{
    threadPool_t m_p;
    threadPoolInit(&m_p, 5, 100, 100);

    for (int idx = 0; idx < 100; idx++)
    {   
        threadPoolAddTask(&m_p, printData, (void *)&idx);
        usleep(50);
    }


    sleep(3);

    threadPoolDestroy(&m_p);
    

    

    return 0;
}