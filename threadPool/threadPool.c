#include "threadPool.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMBER  2
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
    /* 任务队列容量 */
    int queueCapacity;
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
    /* 销毁的线程数 */
    int exitThreadNums;

    /* 锁 - 锁住这个线程池内部的属性 */
    pthread_mutex_t mutexPool;
    /* 锁 - 锁住忙线程的属性 */
    pthread_mutex_t mutexBusy;

    /* 条件变量 - 消费者向生产者发送 目的: 可以继续生产 */
    pthread_cond_t notFull;
    /* 条件变量 - 生产者向消费者发送 目的: 可以继续消费 */
    pthread_cond_t notEmpty;
};

/* 静态函数前置声明 */
static void * thread_Hander(void *arg);
static void * manager_Hander(void *arg);

/* 本质是一个消费者函数 */
static void * thread_Hander(void *arg)
{
    threadPool_t *pool = (threadPool_t * )arg;

    pthread_mutex_lock(&(pool->mutexPool));
    /* 没有任务消费的时候 */
    while (pool->queueSize == 0)
    {
        pthread_cond_wait(&(pool->notEmpty), &(pool->mutexPool));
    }

    /* 从任务队列中取数据 -- 从队列的对头取数据 */
    task_t task = pool->taskQueue[pool->queueFront];
    /* 任务队列的任务数减一. */
    pool->queueSize--;
    /* front 向后移动. */
    pool->queueFront = (pool->queueFront + 1) % pool->queueCapacity;
    pthread_mutex_unlock(&(pool->mutexPool));
    pthread_cond_signal(&(pool->notFull));

    pthread_mutex_lock(&(pool->mutexBusy));
    /* 忙碌的线程数加一. */
    pool->busyThreadNums++;
    pthread_mutex_unlock(&(pool->mutexBusy));

    /* 执行处理函数 */
    task.worker_hander(task.arg);

    pthread_mutex_lock(&(pool->mutexBusy));
    /* 忙碌的线程数加一. */
    pool->busyThreadNums--;
    pthread_mutex_unlock(&(pool->mutexBusy));

    pthread_exit(NULL);
}

static void * manager_Hander(void *arg)
{
    threadPool_t *pool = (threadPool_t * )arg;

    while (1)
    {
        sleep(3);
        pthread_mutex_lock(&(pool->mutexPool));
        int liveThreadNums = pool->liveThreadNums;
        int queueSize = pool->queueSize;
        pthread_mutex_unlock(&(pool->mutexPool));

        pthread_mutex_lock(&(pool->mutexBusy));
        int busyThreadNums = pool->busyThreadNums;
        pthread_mutex_unlock(&(pool->mutexBusy));

        /* 扩大线程池中线程的容量 */
        /* 任务的个数 > 存活的线程个数 && 存活的线程数 < 最大线程数 */
        if (queueSize > liveThreadNums && liveThreadNums < pool->maxThreads)
        {   
            int count = 0;
            /* 加锁 */
            pthread_mutex_lock(&(pool->mutexPool));
            for (int idx = 0; idx < pool->maxThreads && count < NUMBER
                    && pool->liveThreadNums < pool->maxThreads; idx++)
            {
                if (pool->threadIds[idx] == 0)
                {
                    pthread_create(&(pool->threadIds[idx]), NULL, thread_Hander, pool);
                    count++;
                    /* 更新线程池存活的线程数 */
                    pool->liveThreadNums++;
                }
            }
            /* 解锁 */
            pthread_mutex_unlock(&(pool->mutexPool));
        }

        /* 缩小线程池中线程的容量 */
        /* 忙的线程数 * 2 < 存活的线程数 && 存活的线程 > 最小线程数 */
        if (busyThreadNums * 2 < liveThreadNums && liveThreadNums > pool->minThreads)
        {
            pthread_mutex_lock(&(pool->mutexPool));
            pool->exitThreadNums = NUMBER;
            pthread_mutex_unlock(&(pool->mutexPool));

            /* 唤醒等待工作的线程 -- 等待工作的线程就是空闲的线程 */
            for (int idx = 0; idx < NUMBER; idx++)
            {
                /* 发送信号. */
                pthread_cond_signal(&(pool->notEmpty));
            }
        }
    }
    
}


/* 初始化线程池 */
int threadPoolInit(threadPool_t * pool, int minThreadNums, int maxThreadNums, int taskQueueCapacity)
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
        if (taskQueueCapacity < 0) 
        {
            taskQueueCapacity = DEFAULT_MAX_QUEUES;
        }

        pool->minThreads = minThreadNums;
        pool->maxThreads = maxThreadNums;
        /* 队列的容量 */
        pool->queueCapacity = taskQueueCapacity;
        /* 队列任务数大小 */
        pool->queueSize = 0;

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
            ret = pthread_create(&(pool->threadIds[idx]), NULL, thread_Hander, (void *)pool);
            if (ret != 0)
            {
                perror("pthread create error");
                break;
            }
        }

        /* 管理者线程的创建 */
        ret = pthread_create(&pool->managerThread, NULL, manager_Hander, (void *)pool);
        if (ret != 0)
        {
            perror("pthread create error");
            break;
        }
        pool->liveThreadNums = minThreadNums;
        pool->busyThreadNums = 0;
        

        if (
            pthread_mutex_init(&(pool->mutexPool), NULL) != 0 || 
            pthread_mutex_init(&(pool->mutexBusy), NULL) != 0
            )
        {
            perror("mutex error ");
            break;
        }

        if (
            pthread_cond_init(&(pool->notFull), NULL) != 0    || 
            pthread_cond_init(&(pool->notEmpty), NULL) != 0
        )
        {
            perror("cond error ");
            break;
        }

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

    /* 释放锁和条件变量 */
    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_mutex_destroy(&pool->mutexPool);
    pthread_cond_destroy(&(pool->notEmpty));
    pthread_cond_destroy(&(pool->notFull));

    return UNKNOWN_ERROR;
}

/* 添加任务 */
/* 本质上是生产者 */
int threadPoolAddTask(threadPool_t * pool, void *(*worker_hander)(void * arg), void *arg)
{
    if (pool == NULL)
    {
        return NULL_PTR;
    }

    /* 加锁 */
    pthread_mutex_lock(&(pool->mutexPool));
    /* 任务队列满了 */
    while (pool->queueSize == pool->queueCapacity)
    {
        /* 等待条件变量 : 不满的条件变量 */
        pthread_cond_wait(&(pool->notFull), &(pool->mutexPool));
    }

    /* 将新的任务 添加到任务队列中 */
    pool->taskQueue[pool->queueRear].worker_hander = worker_hander;
    pool->taskQueue[pool->queueRear].arg = arg;
    /* 任务个数加一 */
    pool->queueSize++;
    /* 该队列是循环队列 -- 要让此索引循环起来. */
    pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity;

    pthread_mutex_unlock(&(pool->mutexPool));
    pthread_cond_signal(&(pool->notEmpty));

    return ON_SUCCESS;
}