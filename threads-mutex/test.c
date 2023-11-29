#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 100

pthread_mutex_t mutex; // Mutex 선언

void *write_thread(void *filename)
{
    FILE *file = fopen((char *)filename, "a"); // "a"는 파일의 끝에 추가 모드

    if (file == NULL)
    {
        perror("파일을 열 수 없습니다");
        pthread_exit(NULL);
    }

    // Mutex를 사용하여 크리티컬 섹션 보호
    for (int i = 0; i < 5; ++i)
    {
        pthread_mutex_lock(&mutex);

        fprintf(file, "Write Thread: This is line %d.\n", i + 1);
        fflush(file); // 버퍼 비우기

        pthread_mutex_unlock(&mutex);

        sleep(1); // 1초 대기
    }

    fclose(file);
    pthread_exit(NULL);
}

int main()
{
    pthread_t write_thread_id1, write_thread_id2;
    char *filename = "example.txt";

    pthread_mutex_init(&mutex, NULL); // Mutex 초기화

    // 두 개의 스레드 생성
    pthread_create(&write_thread_id1, NULL, write_thread, (void *)filename);
    pthread_create(&write_thread_id2, NULL, write_thread, (void *)filename);

    // 두 개의 스레드 종료 대기
    pthread_join(write_thread_id1, NULL);
    pthread_join(write_thread_id2, NULL);

    pthread_mutex_destroy(&mutex); // Mutex 제거

    return 0;
}
