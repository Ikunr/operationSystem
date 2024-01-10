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
    memset(pipefd, 0, sizeof(pipefd));

    pipe(pipefd);

    /* 查看管道缓冲区函数 */
    long bufferSize = fpathconf(pipefd[0], _PC_PIPE_BUF);
    printf("bufferSize:%ld\n", bufferSize);
    
    bufferSize = fpathconf(pipefd[1], _PC_PIPE_BUF);
    printf("bufferSize:%ld\n", bufferSize);
    return 0;
}