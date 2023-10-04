#include <stdio.h>

int main(void)
{
  FILE *in, *out;
  struct pirate
  {
    char name[100];
    unsigned long booty;
    unsigned int beard_len;
  } p, blackbeard = {"Edward Teach", 950, 48};

  // "data" 파일을 쓰기 모드로 열기
  out = fopen("data", "w");

  // 파일 열기 실패 시 오류 메시지 출력 및 프로그램 종료
  if (!out)
  {
    perror("파일 열기 실패");
    return 1;
  }

  // "blackbeard" 구조체를 "data" 파일에 쓰기
  if (!fwrite(&blackbeard, sizeof(struct pirate), 1, out))
  {
    perror("fwrite");
    return 1;
  }

  // 파일 닫기
  if (fclose(out))
  {
    perror("fclose");
    return 1;
  }

  // "data" 파일을 읽기 모드로 열기
  in = fopen("data", "r");
  if (!in)
  {
    perror("파일 열기 실패");
    return 1;
  }

  // "data" 파일에서 데이터를 읽어 "p" 구조체에 저장
  if (!fread(&p, sizeof(struct pirate), 1, in))
  {
    perror("fread");
    return 1;
  }

  // 파일 닫기
  if (fclose(in))
  {
    perror("fclose");
    return 1;
  }

  // 읽어온 데이터 출력
  printf("name=\"%s\" booty=%lu beard_len=%u\n",
         p.name, p.booty, p.beard_len);

  return 0;
}
