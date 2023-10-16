// atexit 함수를 사용하면 프로그램이 종료될 때 지정된 함수가 호출됩니다.

#include <stdio.h>
#include <stdlib.h>

void out(void)
{
  printf("atexit succeeded!\n");
}

int main(void)
{
  if (atexit(out))
  {
    fprintf(stderr, "atexit failed!\n");
  }
  return 0;
}