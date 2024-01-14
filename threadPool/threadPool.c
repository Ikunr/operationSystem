#include "threadPool.h"
#include <pthread.h>
#include <stdlib.h>

/* 状态码 */
enum STATUS_CODE
{
    ON_SUCCESS,
    NULL_PTR,
    MALLOC_ERROR,
    INVALID_ACCESS,
    THREAD_CREATE_ERR,
    UNKNOWN_ERROR,
};

#define DEFAULT_MIN_THREADS 5
#define DEFAULT_MAX_THREADS 100
#define DEFAULT_MAX_QUEUES 100



/* 任务结点 */
struct task_t
{
    void *(*worker_hander)(void *arg);
    void *arg;
};

/* 线程池 */
struct threadPool_t
{
    /* 任务队列 -- 将之设计成循环队列 */
    struct task_t * taskQueue;
    /* 任务队列任务数大小 */
    int queueSize;
    /* 任务队列的头 */
    int queueFront; 
    /* 任务队列的尾 */
    int queueRear;
    
    /* 工作线程ID */
    pthread_t *threadIds;
    /* 管理着线程 */
    pthread_t *managerThread;

    /* 最小的线程数 */
    int minThreads;
    /* 最大的线程数 */
    int maxThreads;
    /* 忙碌的线程数 */
    int busyThreadNums;
    /* 存活的线程数 */
    int liveThreadNums;
};

/* 静态函数前置声明 */
static void * thread_Hander(void *arg);
static void * manager_Hander(void *arg);

/* 本质是一个消费者函数 */
static void * thread_Hander(void *arg)
{

    pthread_exit(NULL);
}

static void * manager_Hander(void *arg)
{

}


/* 初始化线程池 */
int threadPoolInit(threadPool_t * pool, int minThreadNums, int maxThreadNums, int taskQueueSize)
{
    if (pool == NULL)
    {
        return NULL_PTR;
    }

    /* do ... while 循环 */
    do 
    {
        /* 判断合法性 */
        if (minThreadNums <= 0 || maxThreadNums <= 0 || minThreadNums > maxThreadNums)
        {
            minThreadNums = DEFAULT_MIN_THREADS;
            maxThreadNums = DEFAULT_MAX_THREADS;   
        }
        
        /* 判断合法性 */
        if (taskQueueSize < 0) 
        {
            taskQueueSize = DEFAULT_MAX_QUEUES;
        }

        pool->minThreads = minThreadNums;
        pool->maxThreads = maxThreadNums;
        pool->queueSize = taskQueueSize;

        /* 任务队列 */
        pool->taskQueue = (struct task_t *)malloc(sizeof(struct task_t) * (pool->queueSize));
        if (pool->taskQueue == NULL)
        {
            perror("malloc erorr");
            break;
        }
        /* 循环队列队头位置 */
        pool->queueFront = 0;
        /* 循环队列队尾位置 */
        pool->queueRear = 0;

        pool->threadIds = (pthread_t *)malloc(sizeof(pthread_t) * pool->maxThreads);
        if (pool->threadIds == NULL)
        {
            perror("malloc error");
            break;
        }
        /* 清除脏数据 */
        memset(pool->threadIds, 0, sizeof(pthread_t) * pool->maxThreads);

        int ret = 0;
        /* 工作线程的创建 */
        for (int idx = 0; idx < pool->minThreads; idx++)
        {   
            ret = pthread_create(&(pool->threadIds[idx]), NULL, thread_Hander, NULL);
            if (ret != 0)
            {
                perror("pthread create error");
                break;
            }
        }

        /* 管理者线程的创建 */
        ret = pthread_create(&pool->managerThread, NULL, manager_Hander, NULL);
        if (ret != 0)
        {
            perror("pthread create error");
            break;
        }
        pool->liveThreadNums = minThreadNums;
        pool->busyThreadNums = 0;

        return ON_SUCCESS;
    }while (0);
    
    /* 程序到达这边意味着上面初始化流程出现了错误 */

    /* 释放内存 */
    if (pool->taskQueue != NULL)
    {
        free(pool->taskQueue);
        pool->taskQueue = NULL;
    }

    if (pool->threadIds)
    {
        /* 阻塞回收工作线程资源 */
        for (int idx = 0; idx < pool->minThreads; idx++)
        {
            if (pool->threadIds[idx] != 0)
            {
                pthread_join(pool->threadIds[idx], NULL);
            }
        }

        /* 释放内存 */
        free(pool->threadIds);
        pool->threadIds = NULL;
    }

    /* 阻塞回收管理者线程资源 */
    if (pool->managerThread != 0)
    {
        pthread_join(pool->managerThread, NULL);
    }
    return UNKNOWN_ERROR;
}