#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define PIPE_SIZE   2

int main()
{
    int pipefd[PIPE_SIZE];
    memset(pipefd, 0, sizeof(pipefd));

    /* 创建无名管道 */
    pipe(pipefd);   

    /* 关闭读端 */
    close(pipefd[0]);

    sleep(1);

    int num = 100;
    /* 向没有读端的管道写数据 */
    int ret = write(pipefd[1], (void *)&num, sizeof(int));
    if (ret < 0)
    {
        perror("write error\n");
        exit(-1);
    }
    sleep(10);

    /* 关闭写端 */
    close(pipefd[1]);
    
    return 0;
}