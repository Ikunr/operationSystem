#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 32
int main()
{
    int ret = access("./testfifo", F_OK);
    if (ret == -1)
    {
        /* FIFO文件 不存在 */
        ret = mkfifo("./testfifo", 0644);
        if (ret == -1)
        {   
            perror("mkfifo error");
            _exit(-1);
        }
    }

    int fd = open("./testfifo", O_RDWR);
    if (fd == -1)
    {
        perror("open error");
        _exit(-1);
    }

    char buf[BUFFER_SIZE];
    while(1)
    {
        memset(buf, 0, sizeof(buf));

        // buf = "hello world";
        strncpy(buf, "helloworld", sizeof(buf) - 1);
        write(fd, buf, strlen(buf));

        sleep(3);
    }

    close(fd);
    return 0;
}




