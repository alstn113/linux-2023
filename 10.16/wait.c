// 부모 프로세스가 자식 프로세스가 종료된 후의 skeleton을 확인할 때까지 기다리라고 하는 것이다.

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>

int main(void)
{
  int status;
  pid_t pid;

  if (!fork())
  {
    return 1;
  }

  pid = wait(&status);
  if (pid == -1)
  {
    perror("wait");
  }

  printf("pid = %d\n", pid);

  if (WIFEXITED(status))
  {
    printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
  }

  if (WIFSIGNALED(status))
  {
    printf("killed by signal = %d%s\n", WTERMSIG(status), WCOREDUMP(status) ? "(dumped core)" : "");
  }

  if (WIFSTOPPED(status))
  {
    printf("stopped by signal = %d\n", WSTOPSIG(status));
  }

  if (WIFCONTINUED(status))
  {
    printf("continued\n");
  }

  return 0;
}