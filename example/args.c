#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define LOG(fmt, ...) printf("[%s:%d]" fmt, __FILE__, __LINE__, ##__VA_ARGS__);
void printNum(int count, ...)
{
    va_list ap;
    va_start(ap, count);
    for(int i = 0;i < count; ++i)
    {
        int num = va_arg(ap, int);
        printf("param[%d]:%d\n", i,num);
    }
    va_end(ap);
}

void myprintf(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char* res;
    int ret = vasprintf(&res, fmt, ap);
    if(-1 != ret)
    {
        printf(res);
        free(res);
    }
    va_end(ap);
}

int main()
{
    myprintf("%s-%d\n", "hello", 111);

    return 0;
}