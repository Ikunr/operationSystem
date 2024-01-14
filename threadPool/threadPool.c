#include "threadPool.h"
#include <pthread.h>

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

