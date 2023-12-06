#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int main(void)
{
    cpu_set_t set;
    int ret;
    int i;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    CPU_CLR(1, &set);
    ret = sched_setaffinity(0, sizeof(cpu_set_t), &set);
    if (ret == -1)
        perror("sched_setaffinity");

    for (i = 0; i < CPU_SETSIZE; i++)
    {
        int cpu;
        cpu = CPU_ISSET(i, &set);
        printf("CPU %d is %s\n", i, cpu ? "set" : "unset");
    }
}