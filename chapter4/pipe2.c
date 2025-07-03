// 파이프 통신 파이프 하나로 자식<->부모 양방향 흉내 내보기: 양방향은 파이프 2개 필요함
// 자식 부분 sleep(2) 없으면 충돌혼선
// 자식이 메시지 전송 -> 부모가 읽고 다시 전송 -> 자식이 읽고 출력
#include <stdio.h>
#include <unistd.h>

int main()
{
    int fds[2];                                // 파이프용 파일 디스크립터 (fds[0] = 읽기, fds[1] = 쓰기)
    char str[] = "Who are you?";               // 자식이 먼저 보낼 메시지
    char str2[] = "Thank you for your message."; // 부모가 다음에 보낼 메시지
    char buf[50];                              // 데이터 읽기용 버퍼
    pid_t pid;

    pipe(fds);                                 // 파이프 생성
    pid = fork();                              // 부모-자식 프로세스 분기

    if (pid == 0) {
        // 자식 프로세스
        write(fds[1], str, sizeof(str));       // 부모에게 메시지 보냄
        sleep(2);                              // 잠깐 대기 (부모가 메시지 준비할 시간)
        read(fds[0], buf, sizeof(buf));        // 부모가 보낸 메시지 읽음
        printf("child proc output: %s\n", buf);  // 출력
    }
    else {
        // 부모 프로세스
        read(fds[0], buf, sizeof(buf));        // 자식이 보낸 메시지 읽음
        printf("parent proc output: %s\n", buf);
        write(fds[1], str2, sizeof(str2));     // 자식에게 메시지 응답
        sleep(3);                              // 자식이 읽을 수 있게 대기
    }

    return 0;
}
