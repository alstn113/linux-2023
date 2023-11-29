#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/fs.h>

int main(void)
{
    pid_t pid;
    int i;

    pid = fork();
    if (pid == -1)
    {
        return -1;
    }
    else if (pid != 0)
    {
        exit(EXIT_SUCCESS);
    }

    if (setsid() == -1)
    {
        return -1;
    }

    if (chdir("/workspace/linux-programmnig/src") == -1)
    {
        return -1;
    }

    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);

    int count = 0;
    while (count <= 100)
    {
        printf("%d\n", count);
        sleep(1);
        count++;
    }
}