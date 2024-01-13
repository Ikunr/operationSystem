#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#define SIZE    5

int main()
{
    /* 创建进程 */
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork error");
        _exit(-1);
    }
    else if (pid == 0)
    {
        /* 子进程. */
        int idx = 0;
        for (idx = 0; idx < SIZE; idx++)
        {
            printf("l am child process\n");
            sleep(1);
        }

        /* 获取子进程的ID. */
        printf("child id:%d\n", getpid());
    }
    else
    {
        /* 打印pid. */
        printf("pid:%d\n", pid);

        /* 父进程. */
        printf("l am parent process parent id : %d\n", getpid());
        sleep(2);

        printf("kill sub process now\n");
        kill(pid, SIGINT);
    }


    while(1)
    {
        sleep(3);
    }
    return 0;
}