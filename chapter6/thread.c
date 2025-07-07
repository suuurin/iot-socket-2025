#include <stdio.h>
#include <pthread.h>    // 스레드 관련 함수
#include <unistd.h>

void* thread_main(void*);

int main()
{
    pthread_t t_id;     // 스레드 ID 저장 변수
    int thread_arg = 5; // 스레드에 전달할 인자 (5초동안 출력)

    // 스레드 생성: t_id에 ID 저장, 기본속성(NULL), 실행 함수, 전달 인자
    if(pthread_create(&t_id, NULL, thread_main, &thread_arg) != 0)
    {
        puts("pthread_create() error"); // 생성 실패 시
        return -1;
    }
    sleep(10);  // 메인 함수 10초 대기
    puts("end of main");

    return 0;
}

// 스레드가 실행하는 함수
// void*: 어떤 자료형이든 받기 위해
void* thread_main(void* arg)
{
    int i;
    // void*로 받은 인자 정수 포인터로 형변환 -> *역참조로 값 5 알아내기
    int cnt = *((int*)arg);
    for(i=0; i<cnt; i++) {
        sleep(1);
        puts("running thread");
    }
    return NULL;
}
