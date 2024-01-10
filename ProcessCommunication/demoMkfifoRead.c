#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 32
int main()
{
    int ret = access("./testfifo", F_OK);
    if (ret == -1)
    {
        perror("access error");
        _exit(-1);
    }

    /* 程序执行到这个地方, 说明fifo文件一定存在 */
    int fd = open("./testfifo", O_RDWR | O_NONBLOCK);
    if (fd == -1)
    {
        perror("open error");
        _exit(-1);
    }

    #if 0
    /* 可以在这个地方设置非阻塞 */
    fcntl()
    #endif 

    char buf[BUFFER_SIZE];
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        int bytes = read(fd, buf, sizeof(buf) - 1);
        if (bytes < 0)
        {
            perror("read error");
            /* 关闭文件 */
            close(fd);
            _exit(-1);
        }
        printf ("buf:%s\n", buf);
        sleep(4);
    }

    close(fd);
    return 0;
}




