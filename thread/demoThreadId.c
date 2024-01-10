#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

int  main()
{
    /* 获取调用线程的ID号 */
    pthread_t tid = pthread_self();
    /* 行缓冲 */
    printf ("tid:%ld\n", tid);

    /* 获取进程号 */
    pid_t pid = getpid();
    printf("pid:%d\n", pid);


    /* 判断两个线程号是否一致 */
    int ret = pthread_equal(tid, pthread_self());
    printf("ret:%d\n", ret);


    



    return 0;
}