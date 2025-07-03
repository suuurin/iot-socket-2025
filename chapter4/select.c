#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <string.h>

#define BUF_SIZE 30

int main()
{
    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;

    FD_ZERO(&reads);          // fd_set 초기화
    FD_SET(0, &reads);        // 표준 입력(키보드) 0번 디스크립터 등록

    while (1) {
        temps = reads;        // select() 호출 시 원본 훼손되므로 복사본 사용
        timeout.tv_sec = 5;   // 타임아웃 5초 설정
        timeout.tv_usec = 0;

        result = select(1, &temps, 0, 0, &timeout);  // 키보드 입력 감시
        if (result == -1) {
            puts("select() error");  // select 오류 발생 시
            break;
        } else if (result == 0) {
            puts("time-out");        // 5초 내 입력 없으면 타임아웃 메시지 출력
        } else {
            if (FD_ISSET(0, &temps)) {  // 0번(stdin)에 입력이 들어왔는지 확인
                str_len = read(0, buf, BUF_SIZE);  // 키보드에서 입력 읽음
                buf[str_len] = '\0';                  // 널 문자 추가 (문자열 종료)
                printf("Message from console: %s", buf);
            }
            if (strcmp(buf, "exit\n") == 0 ) break;
        }
    }

    return 0;
}

