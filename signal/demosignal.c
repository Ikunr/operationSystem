#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


/* 捕捉信号 */
void sigHandler(int sig)
{
    printf("sig :%d\n", sig);

    printf("heiheihei\n");
}

int main()
{
    /* 注册信号 */
    
    /* 1. 默认动作 */
    // signal(SIGINT, SIG_DFL);

    /* 2. 忽略 */
    // signal(SIGINT, SIG_IGN);

    /* 3. 自定义处理函数 */
    signal(SIGINT, sigHandler);

    while (1)
    {
        printf("hello world\n");
        sleep(2);
    }


    return 0;
}