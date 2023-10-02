#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 1000 // 단어 최대 길이

// 단어 비교 함수
int compareWords(const void *a, const void *b)
{
  return strcmp(*(const char **)a, *(const char **)b);
}

int main(int argc, char *argv[])
{

  // 출력 파일 열기
  FILE *outputFile = fopen(argv[argc - 1], "w");
  if (!outputFile)
  {
    perror("출력 파일 열기 실패");
    return 1;
  }

  // 입력 파일 개수 계산
  int numInputFiles = argc - 2;

  // 모든 입력 파일에서 단어 읽어오기
  char **words = malloc(sizeof(char *) * MAX_WORD_LENGTH);
  int numWords = 0;

  for (int i = 1; i < argc - 1; i++)
  {
    FILE *inputFile = fopen(argv[i], "r");
    if (!inputFile)
    {
      perror("입력 파일 열기 실패");
      return 1;
    }

    char word[MAX_WORD_LENGTH];
    while (fscanf(inputFile, "%s", word) != EOF) // 파일 끝까지 단어 읽기
    {
      words[numWords] = strdup(word); // strdup으로 단어 복사
      numWords++;                     // 단어 개수 증가
    }

    fclose(inputFile); // 입력 파일 닫기
  }

  // 단어 정렬
  qsort(words, numWords, sizeof(char *), compareWords);

  // 정렬된 단어 출력
  for (int i = 0; i < numWords; i++)
  {
    fprintf(outputFile, "%s ", words[i]); // 파일에 단어 출력
    free(words[i]);                       // strdup으로 할당한 메모리 해제
  }
  fprintf(outputFile, "\n"); // 줄바꿈

  fclose(outputFile); // 출력 파일 닫기
  free(words);        // 단어 배열 메모리 해제

  printf("단어 정렬 및 파일 병합이 완료되었습니다.\n");

  return 0;
}
