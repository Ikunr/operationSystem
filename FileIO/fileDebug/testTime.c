#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>

/* 调试标记是否存在 */
int g_debug = 0;

/* 文件指针 */
FILE *g_logfp = NULL;

#define LOGPR(fmt, args...)         \
    do                              \
    {                               \
        if (g_debug)                \
        {                           \
            time_t now;             \
            struct tm *ptm = NULL;  \
            now = time(NULL);       \
            ptm = localtime(&now);        \
            fprintf(g_logfp, "[file:(%s), func(%s), line(%d), time(%s)]: "fmt,   \
            __FILE__, __FUNCTION__, __LINE__, asctime(ptm), ##args);             \
            fflush(g_logfp);                                                     \
        }                                                                        \
    }while(0)

/* 日志 : 就是文件 */
/* 打开日志文件 /*/
void log_init()
{   
    time_t now;
    
    /* 避免野指针 */
    struct tm *ptm = NULL;
#define DEBUG_FLAG  "./my_debug.flag"
    
    /* access函数 成功返回0 */
    if (access(DEBUG_FLAG, F_OK) == 0)
    {
        g_debug = 1;
    }

    if (!g_debug)
    {
        return;
    }

#define DEBUG_FILE "./my_debug.log"

    if ((g_logfp = fopen(DEBUG_FILE, "a")) == NULL)
    {
        perror("fopen error");
        return;
    }

    now = time(NULL);
    ptm = localtime(&now);

    LOGPR("=====================log init done.====================");
    LOGPR("=================%s", asctime(ptm));
    return; 
}

/* 关闭文件 */
void log_close()
{
    if (g_logfp)
    {
        fclose(g_logfp);
        g_logfp = NULL;
    }
}


int main()
{
    /* 启动日志程序 */
    log_init();

    int count = 50;
    while(count--)
    {
        LOGPR("count = %d\n", count);

        sleep(2);
    }

    /* 关闭文件 */
    log_close();
    return 0;
}

