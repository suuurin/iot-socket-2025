#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void* thread_inc(void*);
void* thread_des(void*);

long long num = 0;
pthread_mutex_t mutex;

int main()
{
    pthread_t thread_id[100];
    int i;

    pthread_mutex_init(&mutex, NULL);   // mutex 생성 초기화

    for(i = 0; i < 100; i++) {
        if(i % 2)
            pthread_create(&thread_id[i], NULL, thread_inc, NULL);
        else
            pthread_create(&thread_id[i], NULL, thread_des, NULL);
    }

    for(i = 0; i < 100; i++)
        pthread_join(thread_id[i], NULL);

    printf("result: %lld\n", num);

    pthread_mutex_destroy(&mutex);      // mutex 제거
    return 0;
}

void* thread_inc(void* arg) {
    int i;
    pthread_mutex_lock(&mutex);         // mutex(key) 획득
    for(i = 0; i < 100000; i++) {
        num += 1;
    }
    pthread_mutex_unlock(&mutex);       // mutex 반납
    return NULL;
}

void* thread_des(void* arg) {
    int i;
    pthread_mutex_lock(&mutex);         // lock
    for(i = 0; i < 100000; i++) {
        num -= 1;
    }
    pthread_mutex_unlock(&mutex);       // unlock
    return NULL;
}

