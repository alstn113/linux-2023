#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/file.h>

#define MAX_WORD_LENGTH 1000 // 단어 최대 길이

int compareWords(const void *a, const void *b)
{
    return strcasecmp(*(const char **)a, *(const char **)b);
}

void writeToOutputFile(FILE *file, const char *word)
{
    // 파일 락 얻기
    flockfile(file);

    // 파일 읽기
    fseek(file, 0, SEEK_END);
    fprintf(file, "%s ", word);

    char buffer[MAX_WORD_LENGTH];
    char **words = malloc(sizeof(char *) * MAX_WORD_LENGTH);
    int numWords = 0;

    while (fscanf(file, "%s", buffer) == 1)
    {
        printf("%s ", buffer);
        words[numWords] = strdup(buffer);
        numWords++;
    }

    qsort(words, numWords, sizeof(char *), compareWords);

    // 파일 쓰기
    fseek(file, 0, SEEK_SET); // 파일 포인터를 파일의 처음으로 이동
    for (int i = 0; i < numWords; i++)
    {
        printf("여기 작동 안해?");
        printf("%s ", words[i]);
        fprintf(file, "%s ", words[i]);
        free(words[i]);
    }
    free(words);

    // 파일 락 해제
    funlockfile(file);
}

void processInputFile(FILE *outputFile, const char *inputFileName)
{
    FILE *inputFile = fopen(inputFileName, "r");
    if (!inputFile)
    {
        perror("입력 파일 열기 실패");
        exit(1);
    }

    char word[MAX_WORD_LENGTH];
    while (fscanf(inputFile, "%s", word) == 1)
    {
        writeToOutputFile(outputFile, word);
    }

    fclose(inputFile);
}

int main(int argc, char *argv[])
{
    // 파일이 없으면 생성
    FILE *outputFile = fopen(argv[argc - 1], "w+");
    if (!outputFile)
    {
        perror("출력 파일 열기 실패");
        return 1;
    }

    // 입력 파일 개수 계산
    int numInputFiles = argc - 2;
    int filesPerProcess = numInputFiles / 2;

    int i;

    for (i = 0; i < 2; i++)
    {
        pid_t child_pid = fork();

        if (child_pid == 0)
        {
            // 자식 프로세스
            int start = i * filesPerProcess;
            int end = (i + 1) * filesPerProcess;

            if (i == 1)
                end = numInputFiles;

            for (int j = start; j < end; j++)
            {
                processInputFile(outputFile, argv[j + 1]);
            }

            fclose(outputFile);
            exit(0); // 자식 프로세스는 쓰기를 마치고 종료
        }
        else if (child_pid < 0)
        {
            perror("fork 실패");
            return 1;
        }
    }

    // 부모 프로세스가 자식 프로세스의 종료를 기다림
    for (i = 0; i < 2; i++)
    {
        wait(NULL);
    }

    fclose(outputFile);

    return 0;
}
