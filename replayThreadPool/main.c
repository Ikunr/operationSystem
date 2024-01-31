#include <stdio.h>
#include "threadPool.h"
#include <stdlib.h>
#include <unistd.h>

void * printData(void *arg)
{
    int val = *(int *)arg;
    printf("thread %ld is working number:%d\n", pthread_self(), val);
    
    /* 休眠1000ms */
    usleep(1000);
    return NULL;
}

int main()
{
    threadpool_t m_p;
    threadPoolInit(&m_p, 3, 10, 100);

    int *nums = (int *)malloc(sizeof(int) * 100);
    if (nums == NULL)
    {
        perror("malloc error\n");
        exit(-1);
    }

    for (int idx = 0; idx < 100; idx++)
    {   
        nums[idx] = idx + 100;
        threadPoolAddTask(&m_p, printData, (void *)&nums[idx]);
    }
    sleep(30);

    /* 自己开辟 自己释放 */
    if (nums)
    {
        free(nums);
        nums = NULL;
    }
    
    /* 释放线程池 */
    threadPoolDestroy(&m_p);
    
}