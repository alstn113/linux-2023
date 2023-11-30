#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_SLEEP 5
#define INIT_BALANCE 1000
#define MAX_SIZE 32
#define MAX_AMOUNT_OF_WITHDRAW 100

struct account
{
    pthread_mutex_t *mutex;
    int balance;
};

struct thread_info
{
    struct account *saving;
    char name[MAX_SIZE];
};

void *start_thread(void *);
int withdraw(struct account *, char *, int);
int disburse_money(int);

int main(void)
{
    pthread_t thread1, thread2;
    pthread_mutex_t the_mutex;

    pthread_mutex_init(&the_mutex, NULL);

    struct account saving;
    saving.balance = INIT_BALANCE;
    saving.mutex = &the_mutex;

    struct thread_info owner1;
    struct thread_info owner2;

    owner1.saving = &saving;
    strcpy(owner1.name, "Owner1");
    owner2.saving = &saving;
    strcpy(owner2.name, "Owner2");

    pthread_create(&thread1, NULL, start_thread, (void *)&owner1);
    pthread_create(&thread2, NULL, start_thread, (void *)&owner2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}

void *start_thread(void *owner)
{
    int i = 0;
    int amount_to_withdraw = 0;

    srand(time(NULL));

    for (i = 0; i < 5; i++)
    {
        sleep(rand() % MAX_SLEEP);
        amount_to_withdraw = rand() % MAX_AMOUNT_OF_WITHDRAW + 1;
        withdraw(((struct thread_info *)owner)->saving, ((struct thread_info *)owner)->name, amount_to_withdraw);
    }

    return ((struct thread_info *)owner)->name;
}

int withdraw(struct account *account, char *name, int amount)
{
    pthread_mutex_lock(account->mutex);

    const int balance = account->balance;
    if (balance < amount)
    {
        pthread_mutex_unlock(account->mutex);
        return -1;
    }

    account->balance = balance - amount;
    printf("%s withdraw %d\n", name, amount);
    pthread_mutex_unlock(account->mutex);

    disburse_money(account->balance);

    return 0;
}

int disburse_money(int balance)
{
    printf("Current balance: %d\n", balance);
    return 0;
}