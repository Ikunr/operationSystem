#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#define SIZE    10

int main()
{
    for (int idx = 0; idx < SIZE; idx++)
    {
        if (idx == 5)
        {
            /* 给当前进程发送Ctrl + C*/
            raise(SIGINT);
            /* 等价于: kill(getpid(), SIGINT); */
        }
        printf("idx = %d\n", idx);
        sleep(1);
    }
}