#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void * thread_func(void *arg)
{
    /* 线程分离 */
    pthread_t tid = pthread_self();
    pthread_detach(tid);

    printf("thread func...\n");

    sleep(100);
}


/* */
int main()
{
    pthread_t tid;
    /* 创建线程 */
    int ret = pthread_create(&tid, NULL, thread_func, NULL);
    if (ret != 0)
    {
        char * err = strerror(ret);
        perror(err);
        _exit(-1);
    }
    sleep(1);
    /* 等待回收线程资源 */
    ret = pthread_join(tid, NULL);
    if (ret != 0)
    {
        printf("ret:%d\n", ret, EINVAL);
        _exit(-1);
    }

    sleep(1);
    return 0;
}