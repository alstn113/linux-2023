// 제공된 코드는 새로운 프로세스를 생성하고 그 프로세스 내에서
// 다른 프로그램("windlass")을 실행하는 간단한 예제입니다.

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  pid_t pid;

  pid = fork();

  if (pid == -1)
  {
    perror("fork");
  }

  if (!pid)
  {
    const char *args[] = {"windlass", NULL};
    int ret;

    ret = execv("/bin/windlass", args);
    if (ret == -1)
    {
      perror("execv");
      exit(EXIT_FAILURE);
    }
  }
}