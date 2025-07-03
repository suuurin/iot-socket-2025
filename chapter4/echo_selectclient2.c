#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;

    fd_set reads, cpy_reads;
    struct timeval timeout;

    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else
        puts("Connected..........");

    // 소켓과 표준 입력을 감시할 fd_set 구성
    FD_ZERO(&reads);
    FD_SET(0, &reads);     // 표준 입력(stdin)
    FD_SET(sock, &reads);  // 서버 소켓

    while (1) {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        int result = select(sock + 1, &cpy_reads, 0, 0, &timeout);
        if (result == -1) break;
        if (result == 0) continue;

        // 키보드 입력 감지
        if (FD_ISSET(0, &cpy_reads)) {
            fgets(message, BUF_SIZE, stdin);
            if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
                break;

            write(sock, message, strlen(message));
        }

        // 서버 메시지 감지
        if (FD_ISSET(sock, &cpy_reads)) {
            str_len = read(sock, message, BUF_SIZE - 1);
            if (str_len == 0) break;  // 서버 종료
            message[str_len] = 0;
            printf("%s", message);
            fflush(stdout);
        }
    }

    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
