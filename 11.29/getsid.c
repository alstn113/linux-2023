#include <stdio.h>
#include <unistd.h>

int main(void)
{
    pid_t pid, sid, pgid;

    // 현재 프로세스의 PID를 얻어와 출력
    pid = getpid();
    if (pid == -1)
    {
        perror("getpid");
        return 1; // 프로그램 종료
    }
    printf("PID: %d\n", pid);

    // 현재 프로세스의 세션 ID를 얻어와 출력
    sid = getsid(0);
    if (sid == -1)
    {
        perror("getsid");
        return 1;
    }
    printf("Session ID: %d\n", sid);

    // 현재 프로세스의 프로세스 그룹 ID를 얻어와 출력
    pgid = getpgid(0);
    if (pgid == -1)
    {
        perror("getpgid");
        return 1;
    }
    printf("Process Group ID: %d\n", pgid);

    return 0;
}
