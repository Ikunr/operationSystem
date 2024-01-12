#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/* 锁 */
pthread_mutex_t g_mutex;


// 打印机，公共资源
void printer(char *str)
{
    /* 加锁 */
    pthread_mutex_lock(&g_mutex);
    while (*str != '\0')
    {
        putchar(*str);
        fflush(stdout);
        str++;

        /* 休眠是为了:让出CPU. */
        usleep(100);
    }
    /* 解锁 */
    pthread_mutex_unlock(&g_mutex);
    printf("\n");
}

// 线程一
void *thread_fun_1(void *arg)
{
    char *str = "hello";
    printer(str); //打印
}

// 线程二
void *thread_fun_2(void *arg)
{
    // usleep(1);
    char *str = "world";
    printer(str); //打印
}

int main()
{ 
    /* 参数2: 锁默认属性 */
    pthread_mutex_init(&g_mutex, NULL);

    pthread_t tid1, tid2;

    // 创建 2 个线程
    pthread_create(&tid1, NULL, thread_fun_1, NULL);
    pthread_create(&tid2, NULL, thread_fun_2, NULL);

    // 等待线程结束，回收其资源
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);


    /* 释放锁资源 : 必须解锁之后才可以操作此函数. */
    int res = pthread_mutex_destroy(&g_mutex);
    if (res == EBUSY)
    {
        printf("pthread_mutex_destroy error\n");
        exit(-1);
    }


    return 0;
}