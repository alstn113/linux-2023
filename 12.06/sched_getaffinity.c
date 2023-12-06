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
    // 구름 IDE가 4개의 CPU를 가지고 있어서 0-4까지 set임
    ret = sched_getaffinity(0, sizeof(cpu_set_t), &set);
    if (ret == -1)
        perror("sched_getaffinity");

    for (i = 0; i < CPU_SETSIZE; i++)
    {
        int cpu;
        cpu = CPU_ISSET(i, &set);
        printf("CPU %d is %s\n", i, cpu ? "set" : "unset");
    }
}