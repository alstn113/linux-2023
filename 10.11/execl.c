// vi 실행하기

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  int ret;
  ret = execl("/usr/bin/vi", "vi", NULL);

  if (ret == -1) // if문을 사용하여 오류 검사
  {
    perror("execl"); // perror 함수를 호출하여 오류 메시지 출력
  }

  return 0;
}
