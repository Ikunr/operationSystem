#include <stdio.h>
#include <string.h>

/* 模拟段错误 信号SIGSEGV. */
int main()
{
    char * ptr = NULL;

    strncpy(ptr, "hello world", strlen("hello world"));
    
    return 0;
}