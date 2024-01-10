#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>


typedef struct StuInfo
{
    int age;
    char sex;
} StuInfo;

/* 全局变量 */
int g_retval = 100;

void * thread_func(void *arg)
{   
    StuInfo *info = (StuInfo *)arg;
    printf("age:%d,\t sex:%c\n", info->age, info->sex);
    printf("l am new thread\n");   


    pthread_t tid = pthread_self();
    printf("thread tid:%ld\n", tid);

    sleep(1);
    
#if 0
    /* 栈空间 离开函数 栈空间的地址就被释放了。 */
    int retval = 0;
#endif 
    pthread_exit((void *)&g_retval);
}

int main()
{
    pthread_t tid;

    StuInfo stu;
    stu.age = 10;
    stu.sex = 'f';

    /* 创建线程 */
    int ret = pthread_create(&tid, NULL, thread_func, (void *)&stu);
    if (ret != 0)
    {
        char * err = strerror(ret);
        perror(err);
        _exit(-1);
    }
    printf("main tid %ld\n", tid);

    /* 阻塞等待回收线程资源 */
    int *retVal = NULL;
    ret = pthread_join(tid, (void **)&retVal);
    if (ret != 0)
    {
        printf("pthread_join error\n");
        exit(-1);
    }
    printf("retVal:%d\n", *retVal);

    printf("hello world\n");
    sleep(300);
    return 0;
}
