#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(void)
{
    int ret;
    int val;

    printf("Current nice value is %d\n", nice(0));
    val = nice(0);

    val = 10 - val;
    errno = 0;
    ret = nice(val);
    if (ret == -1 && errno != 0)
        perror("nice");
    else
        printf("nice value now %d\n", ret);
}