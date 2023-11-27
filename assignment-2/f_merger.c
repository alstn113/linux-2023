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

// outputFile에 단어를 삽입하고 정렬하는 함수
void insertWordInFile(FILE *outputFile, const char *word)
{
    // 파일 락 얻기
    flockfile(outputFile);

    // 읽기용과 쓰기용 두 개의 임시 파일 생성
    FILE *tempReadFile = tmpfile();
    FILE *tempWriteFile = tmpfile();

    if (!tempReadFile || !tempWriteFile)
    {
        perror("Failed to create temporary files");
        funlockfile(outputFile);
        return;
    }

    char fileWord[MAX_WORD_LENGTH];

    // 기존 파일의 내용을 읽어 정렬된 상태로 tempReadFile에 기록
    while (fscanf(outputFile, "%s", fileWord) == 1)
    {
        fprintf(tempReadFile, "%s ", fileWord);
    }

    // tempReadFile에서 단어를 읽어 정렬된 상태로 tempWriteFile에 기록
    char **words = malloc(MAX_WORD_LENGTH * sizeof(char *));
    int wordCount = 0;

    fseek(tempReadFile, 0, SEEK_SET);
    while (fscanf(tempReadFile, "%s", fileWord) == 1)
    {
        words[wordCount++] = strdup(fileWord);
    }

    // 새로운 단어를 words 배열에 추가
    words[wordCount++] = strdup(word);

    // words 배열을 정렬
    qsort(words, wordCount, sizeof(char *), compareWords);

    // 정렬된 내용을 tempWriteFile에 쓰기
    for (int i = 0; i < wordCount; i++)
    {
        fprintf(tempWriteFile, "%s ", words[i]);
        free(words[i]);
    }

    free(words);

    // Move the content from the tempWriteFile back to the outputFile
    fseek(tempWriteFile, 0, SEEK_SET);
    int ch;
    while ((ch = fgetc(tempWriteFile)) != EOF)
    {
        fputc(ch, outputFile);
    }

    fclose(tempReadFile);
    fclose(tempWriteFile);

    // 파일 락 해제
    funlockfile(outputFile);
}

// 프로세스가 한 파일을 핸들링하는 부분
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
        // 단어를 파일에 삽입하고 정렬하기
        insertWordInFile(outputFile, word);
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

    // oututFile 정렬하기
    char **words = malloc(MAX_WORD_LENGTH * sizeof(char *));
    int wordCount = 0;

    fseek(outputFile, 0, SEEK_SET);
    char fileWord[MAX_WORD_LENGTH];
    while (fscanf(outputFile, "%s", fileWord) == 1)
    {
        words[wordCount++] = strdup(fileWord);
    }

    qsort(words, wordCount, sizeof(char *), compareWords);

    // 정렬된 내용을 파일에 쓰기
    fseek(outputFile, 0, SEEK_SET);
    for (int i = 0; i < wordCount; i++)
    {
        fprintf(outputFile, "%s ", words[i]);
        free(words[i]);
    }

    free(words);

    fclose(outputFile);

    return 0;
}
