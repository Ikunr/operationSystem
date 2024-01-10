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

void * thread_func1(void *arg)
{   
    StuInfo *info = (StuInfo *)arg;
    printf("thread1 age:%d,\t sex:%c\n", info->age, info->sex);

    sleep(1);
    
    pthread_exit(NULL);
}

void * thread_func2(void *arg)
{   
    int val = *(int *)arg;
    printf("thread2 val:%d\n", val);
    sleep(1);
    
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid1;
    pthread_t tid2;


    StuInfo stu;
    stu.age = 10;
    stu.sex = 'f';

    /* 创建线程 */
    int ret = pthread_create(&tid1, NULL, thread_func1, (void *)&stu);
    if (ret != 0)
    {
        char * err = strerror(ret);
        perror(err);
        _exit(-1);
    }

    int num = 100;
    ret = pthread_create(&tid2, NULL, thread_func2, (void *)&num);
    if (ret != 0)
    {
        char * err = strerror(ret);
        perror(err);
        _exit(-1);
    }

    /* 阻塞等待回收线程资源 */
    ret = pthread_join(tid1, NULL);
    if (ret != 0)
    {
        printf("pthread_join error\n");
        exit(-1);
    }
    ret = pthread_join(tid2, NULL);
    if (ret != 0)
    {
        printf("pthread_join error\n");
        exit(-1);
    }
    

    printf("hello world\n");
    sleep(300);
    return 0;
}
