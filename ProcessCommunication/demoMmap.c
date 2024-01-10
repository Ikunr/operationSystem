#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>


int main()
{
    /* 打开文件. */
    int fd = open("./mmap.txt", O_RDWR | O_CREAT, 0644);
    if (fd == -1)
    {
        perror("open error");
        /* 进程退出 */
        _exit(-1);
    }
    
    void * ptr = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap error");
        /* 进程退出 */
        _exit(-1);
    }

    /* 关闭文件 */
    close(fd);

    /* 从已经映射好的内存里面 读取数据 */
    printf("ptr:%s", (char *)ptr);
    

    strcpy((char *)ptr, "666");

    /* 解除映射 */
    int ret = munmap(ptr, 1024);
    if (ret == -1)
    {
        perror("munmap error");
        _exit(-1);
    }

    return 0;
}