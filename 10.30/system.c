#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/resource.h>

int my_system(const char *cmd)
{
  int status;
  pid_t pid;

  pid = fork();
  if (pid == -1)
    return -1;

  else if (pid == 0)
  {
    const char *argv[4];

    argv[0] = "sh";
    argv[1] = "-c";
    argv[2] = cmd;
    argv[3] = NULL;

    execv("/bin/sh", argv);

    exit(-1);
  }

  if (waitpid(pid, &status, 0) == -1)
    return -1;

  else if (WIFEXITED(status))
    return WEXITSTATUS(status);

  return -1;
}

int main(void)
{
  my_system("ls -l");
  return 0;
}