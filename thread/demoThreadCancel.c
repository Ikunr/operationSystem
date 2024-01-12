#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

void *thread_func(void *arg)
{
    /* 线程分离 */
    pthread_detach(pthread_self());

    while (1)
    {
        printf("l am thread_func...\n");
        sleep(1);
    }
    printf("hello world\n");

    /* 线程退出 */
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thread_func, NULL);
    if (ret != 0)
    {
        printf("thread create error\n");
        _exit(-1);
    }

    sleep(3);
    pthread_cancel(tid);

    


    while (1)
    {
        sleep(3);
    }

    return 0;
}