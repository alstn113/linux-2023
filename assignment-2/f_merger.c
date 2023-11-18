#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/file.h>

#define MAX_WORD_LENGTH 1000 // 단어 최대 길이

// 단어 비교 함수
int compareWords(const void *a, const void *b)
{
    return strcasecmp(*(const char **)a, *(const char **)b);
}

void writeToSharedFile(const char *message, FILE *file)
{
    // 파일 락 얻기
    flock(fileno(file), LOCK_EX);

    // 파일에 쓰기
    fprintf(file, "%s", message);
    fflush(file); // 버퍼 비우기

    // 파일 락 해제
    flock(fileno(file), LOCK_UN);
}

void sortAndWriteToFile(const char *fileName)
{
    // 파일 읽기
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        perror("입력 파일 열기 실패");
        exit(1);
    }

    // 파일 내용 읽어오기
    char **words = NULL;
    char word[MAX_WORD_LENGTH];
    int numWords = 0;

    while (fscanf(file, "%s", word) == 1)
    {
        words = realloc(words, (numWords + 1) * sizeof(char *));
        words[numWords] = strdup(word);
        numWords++;
    }

    fclose(file);

    // 정렬
    qsort(words, numWords, sizeof(char *), compareWords);

    // 파일 쓰기
    file = fopen(fileName, "w");
    if (!file)
    {
        perror("출력 파일 열기 실패");
        exit(1);
    }

    for (int i = 0; i < numWords; i++)
    {
        fprintf(file, "%s ", words[i]);
        free(words[i]);
    }

    free(words);

    fclose(file);
}

int main(int argc, char *argv[])
{
    // 파일이 없으면 생성
    FILE *outputFile = fopen(argv[argc - 1], "w");
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
                FILE *inputFile = fopen(argv[j + 1], "r");
                if (!inputFile)
                {
                    perror("입력 파일 열기 실패");
                    return 1;
                }

                char word[MAX_WORD_LENGTH];
                while (fscanf(inputFile, "%s", word) == 1)
                {
                    writeToSharedFile(word, outputFile);
                }

                fclose(inputFile);
            }

            // 파일을 정렬하고 다시 쓰기
            sortAndWriteToFile(argv[argc - 1]);

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

    printf("부모 프로세스 종료\n");

    fclose(outputFile);

    return 0;
}
