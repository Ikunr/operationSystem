#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

/* 静态加锁. */
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

/* 余量: 甜甜圈 */
int g_margin = 10;

/* 线程共享全局变量 */

void *thread_func1(void * arg)
{
    /* 如果 余量> 0*/
    while (1)
    {
        /* 休眠的时候,会将CPU资源让出去 */
        // usleep(1000);
        
        /* 加锁 */
        pthread_mutex_lock(&g_mutex);
        if (g_margin > 0)
        {
            usleep(100);
            g_margin -= 1;
        }
        /* 解锁 */
        pthread_mutex_unlock(&g_mutex);

        printf("thread1 num:%d\n", g_margin);

        if (g_margin <= 0)
        {
            break;
        }
    }
    printf("thread1 num:%d\n", g_margin);
    pthread_exit(NULL);
}

void *thread_func2(void * arg)
{
    usleep(50);
    /* 如果 余量> 0*/
    while (1)
    {
        /* 加锁 */
        pthread_mutex_lock(&g_mutex);
        if (g_margin >= 2)
        {
            usleep(2000);
            g_margin -= 2;
        }
        /* 解锁 */
        pthread_mutex_unlock(&g_mutex);
        printf("thread2 num:%d\n", g_margin);

        if (g_margin <= 0)
        {
            break;
        }
    }
    printf("thread2 num:%d\n", g_margin);
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


    /* 释放锁资源 */
    pthread_mutex_destroy(&g_mutex);


    return 0;
}