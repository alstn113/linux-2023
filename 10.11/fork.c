#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  pid_t pid;

  pid = fork();

  if (pid > 0)
  {
    printf("Parent process\n");
    printf("Parent process ID: %d\n", getpid());
    printf("Child process ID: %d\n", pid);
  }
  else if (!pid)
  {
    printf("Child process\n");
    printf("Parent process ID: %d\n", getppid());
    printf("Child process ID: %d\n", getpid());
  }
  else
  {
    perror("fork");
    return 1;
  }

  return 0;
}