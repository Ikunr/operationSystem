#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>


/* 线程独享栈空间 */

void *thread_func1(void * arg)
{
    int num = *(int *)arg;
    printf("thread1 num:%d\n", num);

    num += 100;
    printf("thread1 num:%d\n", num);
    pthread_exit(NULL);
}

void *thread_func2(void * arg)
{
    sleep(1);
    int num = *(int *)arg;
    printf("thread2 num:%d\n", num);
    pthread_exit(NULL);
}


int main()
{
    int num = 100;

    pthread_t tid1;
    pthread_t tid2;

    int ret = pthread_create(&tid1, NULL, thread_func1, (void *)&num);
    if (ret != 0)
    {
        printf("thread_create error\n");
        exit(-1);
    }

    ret = pthread_create(&tid2, NULL, thread_func2, (void *)&num);
    if (ret != 0)
    {
        printf("thread_create error\n");
        exit(-1);
    }

    /* 主线程回收子线程的资源 */
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);




    return 0;
}