#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;

    if (argc != 3) {
        // 사용법 안내
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // 소켓 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    // 서버 주소 구조체 초기화
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);   // IP 주소 설정
    serv_adr.sin_port = htons(atoi(argv[2]));        // 포트 번호 설정

    // 서버에 연결 요청
    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else
        puts("Connected..........");

    // 메시지 전송 루프
    while (1) {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        // 종료 조건: "q" 또는 "Q" 입력
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        // 메시지 서버에 전송
        write(sock, message, strlen(message));

        // 서버로부터 응답 수신
        str_len = read(sock, message, BUF_SIZE - 1);
        message[str_len] = 0;  // 문자열 끝에 널 문자 추가
        printf("Message from server: %s", message);
    }

    close(sock);  // 소켓 닫기
    return 0;
}

// 오류 출력 함수
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
