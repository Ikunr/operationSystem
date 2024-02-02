#include <stdio.h>
#include <stdlib.h>
#include "pool.h"
#include <unistd.h>
#include <pthread.h>

void taskFunc(void* arg)
{
    int num = *(int*)arg;
    printf("thread %ld is working, number = %d\n",
        pthread_self(), num);
    usleep(5000);
}

int main()
{
    // 创建线程池
    ThreadPool* pool = threadPoolCreate(3, 10, 100);

    int* num = (int*)malloc(sizeof(int) * 100);

    for (int i = 0; i < 100; ++i)
    {
        num[i] = i + 100;
        threadPoolAdd(pool, taskFunc, &num[i]);
    }

    sleep(20);

    
    

    threadPoolDestroy(pool);

    if (num)
    {
        free(num);
        num = NULL;
    }
    return 0;
}

