#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int main(void)
{
    int policy = sched_getscheduler(0);

    switch (policy)
    {
    case SCHED_OTHER:
        printf("policy - nonal\n");
        break;
    case SCHED_FIFO:
        printf("SCHED_FIFO (First In, First Out)\n");
        break;
    case SCHED_RR:
        printf("SCHED_RR (Round Robin)\n");
        break;
    default:
        fprintf(stderr, "Unknown policy!\n");
    }

    return 0;
}
