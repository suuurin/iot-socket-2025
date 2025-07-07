// 스레드 문제 - 동기화 기법 mutex 등으로 해결
#include <stdio.h>
#include <pthread.h>

long long num = 0;  // 공유 변수 (스레드 간 충돌 주의)

// 증감 함수 선언
void* thread_inc(void* arg);
void* thread_des(void* arg);

int main()
{
    pthread_t thread_id[100];  // 스레드 ID 배열

    // 스레드 100개 생성 (홀수: 증가, 짝수: 감소)
    for(int i = 0; i < 100; i++) {
        if(i % 2)
            pthread_create(&thread_id[i], NULL, thread_inc, NULL);
        else
            pthread_create(&thread_id[i], NULL, thread_des, NULL);
    }

    // 모든 스레드 종료 대기
    for(int i = 0; i < 100; i++)
        pthread_join(thread_id[i], NULL);

    // 최종 결과 출력
    printf("result: %lld\n", num);

    return 0;
}

// num을 1씩 증가시키는 스레드 함수
void* thread_inc(void* arg)
{
    for(int i = 0; i < 500; i++) {
        num += 1;
    }
    return NULL;
}

// num을 1씩 감소시키는 스레드 함수
void* thread_des(void* arg)
{
    for(int i = 0; i < 500; i++) {
        num -= 1;
    }
    return NULL;
}

// 동일하게 증가, 감소시키는데 결과가 0이 아닌 문제
