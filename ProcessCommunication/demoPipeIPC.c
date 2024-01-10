#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define PIPE_SIZE 2

int main()
{
    int pipefd[PIPE_SIZE];
    /* 清除脏数据 */
    memset(pipefd, 0, sizeof(int) * PIPE_SIZE);

    /* 创建管道 */
    pipe(pipefd);

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        exit(-1);
    }

    if (pid == 0)
    {
        /* 关闭子进程的读端 */
        close(pipefd[0]);

        /* 子进程 写 */
        sleep(5);
        int num = 200;
        write(pipefd[1], (void *)&num, sizeof(int));

        #if 0
        /* 这个用法不对. */
        int childReadNum = 0;
        read(pipefd[0], (void *)&childReadNum, sizeof(childReadNum));
        printf("child process : childReadNum:%d\n", childReadNum);
        #endif

        /* 关闭写端 */
        close(pipefd[1]);
    }
    else if (pid > 0)
    {
        sleep(1);

        /* 关闭写端 */
        close(pipefd[1]);
#if 1
        /* 父进程 读 */

        /* 把read设置成非阻塞 */
        
        
        /* 1. 拿到当前文件描述符的标记 */
        int flag = fcntl(pipefd[0], F_GETFL);
        // printf("flag: %d\n", flag);
        /* 2. 或上O_NONBLOCK */
#if 0
        flag = flag | O_NONBLOCK;
#else
        flag |= O_NONBLOCK;
        // printf("flag: %d\n", flag);
#endif
        /* 3. 用新的标记设置文件描述符 */
        int ret = fcntl(pipefd[0], F_SETFL, flag);
        if (ret == -1)
        {
            perror("fcntl setfd error");
            /* 关闭读端 */
            close(pipefd[0]);
            _exit(0);
        }

        int readNum = 0;
        int bytes = read(pipefd[0], (void *)&readNum, sizeof(int));
        printf("bytes:%d\n", bytes);
        // printf("parent process : readNum:%d\n", readNum);
#endif  

        /* 关闭读端 */
        close(pipefd[0]);
        _exit(0);
    }

    while (1)
    {
        sleep(3);
    }
}
