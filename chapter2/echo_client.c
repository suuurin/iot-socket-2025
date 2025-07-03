// echo_client.c - 서버에 메시지를 보내고, 받은 응답을 그대로 출력하는 TCP 클라이언트
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    ssize_t recv_len;

    if (argc != 3) {
        printf("사용법: %s <서버 IP> <PORT>\n", argv[0]);
        exit(1);
    }

    // 1. 소켓 생성 (IPv4, TCP)
    sock_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket failed");
        exit(1);
    }

    // 2. 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);  // 입력한 IP 문자열을 정수형 주소로 변환
    server_addr.sin_port = htons(atoi(argv[2]));       // 문자열 포트 번호 → 정수 → 네트워크 바이트 순서

    // 3. 서버에 연결 시도
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect failed");
        close(sock_fd);
        exit(1);
    }

    printf("서버 연결 완료. 메시지를 입력하세요 (exit: 종료)\n");

    while (1) {
        printf("입력 > ");
        fgets(buffer, BUFFER_SIZE, stdin);  // 사용자 입력

        if (strcmp(buffer, "exit\n") == 0)
            break;

        // 4. 서버에 메시지 전송
        send(sock_fd, buffer, strlen(buffer), 0);

        // 5. 서버로부터 echo 메시지 수신
        recv_len = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);
        if (recv_len <= 0) {
            printf("서버와의 연결이 종료되었습니다.\n");
            break;
        }

        buffer[recv_len] = '\0';
        printf("서버 응답: %s", buffer);
    }

    close(sock_fd);
    return 0;
}
