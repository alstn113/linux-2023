#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_WORD_LENGTH 1000

pthread_mutex_t lock;

typedef struct
{
    FILE *outputFile;
    char *inputFileName;
} ThreadData;

void insertWordInFile(FILE *outputFile, const char *word)
{
    pthread_mutex_lock(&lock);

    fprintf(outputFile, "%s ", word);

    pthread_mutex_unlock(&lock);
}

void *processInputFile(void *arg)
{
    ThreadData *threadData = (ThreadData *)arg;
    FILE *inputFile = fopen(threadData->inputFileName, "r");

    if (!inputFile)
    {
        perror("입력 파일 열기 실패");
        pthread_exit(NULL);
    }

    char word[MAX_WORD_LENGTH];
    while (fscanf(inputFile, "%s", word) == 1)
    {
        insertWordInFile(threadData->outputFile, word);
    }

    fclose(inputFile);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    FILE *outputFile = fopen("output.txt", "w+");
    if (!outputFile)
    {
        perror("출력 파일 열기 실패");
        return 1;
    }

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        perror("Mutex 초기화 실패");
        return 1;
    }

    int numInputFiles = argc - 2;
    int filesPerThread = numInputFiles / 2;

    pthread_t threads[2];
    ThreadData threadData[2];

    for (int i = 0; i < 2; i++)
    {
        int start = i * filesPerThread;
        int end = (i + 1) * filesPerThread;

        if (i == 1)
            end = numInputFiles;

        threadData[i].outputFile = outputFile;
        threadData[i].inputFileName = argv[start + 1];

        if (pthread_create(&threads[i], NULL, processInputFile, &threadData[i]) != 0)
        {
            perror("스레드 생성 실패");
            return 1;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("스레드 대기 실패");
            return 1;
        }
    }

    pthread_mutex_destroy(&lock);

    fclose(outputFile);

    return 0;
}
