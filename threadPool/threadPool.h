#ifndef __THREAD_POOL_H_
#define __THREAD_POOL_H_

/* 任务结点 */
typedef struct task_t task_t;
/* 线程池 */
typedef struct threadPool_t threadPool_t;


/* 初始化线程池 */
int threadPoolInit(threadPool_t * pool, int minThreadNums, int maxThreadNums, int taskQueueSize);

/* 销毁线程池 */
int threadPoolDestroy(threadPool_t * pool);

/* 添加任务 */
int threadPoolAddTask(threadPool_t * pool, void *(*worker_hander)(void * arg), void *arg);

#endif //__THREAD_POOL_H_