// 파이프 두 개로 입출력을 분리해 부모-자식간 양방향 통신 해보기
#include <stdio.h>
#include <unistd.h>


int main()
{
    int fds[2], fds2[2];
    char str[] = "Who are you?";
    char str2[] = "Thank you";
    char buf[30];                   // 데이터 읽기용 버퍼
    pid_t pid;

    pipe(fds), pipe(fds2);      // 파이프 fds(자식->부모) / fds2(부모->자식)
    pid = fork();               // fork(): 프로세스 복사해 자식 프로세스 만들기

    if(pid == 0) {  // 자식은 분기했을 때 pid 0 받음
        // 자식 프로세스
        write(fds[1], str, sizeof(str));    // fds[1] = 자식->부모 쓰기
        read(fds2[0], buf, sizeof(buf));    // fds[0] = 부모->자식 읽기
        printf("childs: %s\n", buf);        // 부모에게 받은 메시지 출력
    } else {
        // 부모 프로세스
        read(fds[0], buf, sizeof(buf));
        printf("parent: %s\n", buf);        // 자식에게 받은 메시지 출력
        write(fds2[1], str2, sizeof(str2));
        sleep(3);                           // 자식이 읽고 출력할 시간 확보
                                            // 부모가 쓰고 바로 닫히면 출력끊김/자식 읽기 전 파이프 닫힘
    }
}
