#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/* 信号量头文件. */
#include <semaphore.h>

/* 信号量 */
sem_t g_sem;

// 打印机，公共资源
void printer(char *str)
{
    /* P操作 */
    sem_wait(&g_sem);
    while (*str != '\0')
    {
        putchar(*str);
        fflush(stdout);
        str++;

        /* 休眠是为了:让出CPU. */
        usleep(100);
    }
    /* V操作 */
    sem_post(&g_sem);

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
    /* 初始化信号量 */
    sem_init(&g_sem, 0, 1);

    pthread_t tid1, tid2;

    // 创建 2 个线程
    pthread_create(&tid1, NULL, thread_fun_1, NULL);
    pthread_create(&tid2, NULL, thread_fun_2, NULL);

    // 等待线程结束，回收其资源
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    /* 销毁信号量 */
    sem_destroy(&g_sem);


    return 0;
}