// 파이프 통신 파이프 하나로 자식->부모(단방향)
// 자식이 메시지 전송 ->  부모가 읽고 출력
#include <stdio.h>
#include <unistd.h>

int main()
{
    int fds[2];                     // 읽기/쓰기용 파일 디스크립터 저장 배열
    char str[] = "Who are you?";    // 자식이 보낼 문자열
    char buf[30];                   // 부모가 읽을 버퍼
    pid_t pid;

    pipe(fds);      // pipe(): 파이프 채널 생성, 읽기 fds[0] / 쓰기 fds[1]
    pid = fork();   // 부모 자식 프로세스 분기

    if(pid == 0) write(fds[1], str, sizeof(str));   // 자식. 파이프 쓰기 송신
    else {  // 부모 프로세스
        read(fds[0], buf, sizeof(buf)); // 파이프 읽기 데이터 수신
        puts(buf);                      // 받은 데이터 출력
    }
    return 0;

}
