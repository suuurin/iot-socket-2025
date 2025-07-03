#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *buf);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;

    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // 서버 소켓 생성 및 바인딩
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    // 읽기 소켓 리스트 초기화
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);  // 서버 소켓을 읽기 감시 대상에 추가
    fd_max = serv_sock;

    while (1) {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout);
        if (fd_num == -1) break;
        if (fd_num == 0) continue;

        for (i = 0; i <= fd_max; i++) {
            if (FD_ISSET(i, &cpy_reads)) {
                if (i == serv_sock) {
                    // 새 클라이언트 접속
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                    FD_SET(clnt_sock, &reads);
                    if (clnt_sock > fd_max)
                        fd_max = clnt_sock;
                    printf("connected client: %d \n", clnt_sock);
                } else {
                    // 기존 클라이언트 메시지 처리
                    str_len = read(i, buf, BUF_SIZE);
                    if (str_len == 0) {
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client: %d \n", i);
                    } else {
                        buf[str_len] = '\0';
                        printf("client %d 보냄: %s", i, buf);

                        // 모든 클라이언트에 발신자 + 메시지 전송
                        char send_buf[BUF_SIZE + 50];   // 전송용 버퍼
                        snprintf(send_buf, sizeof(send_buf), "%d send: %s", i, buf);

                        for (int j = 0; j <= fd_max; j++) {
                            if (FD_ISSET(j, &reads) && j != serv_sock && j != i) {
                                write(j, send_buf, strlen(send_buf));
                            }
                        }
                    }
                }
            }
        }
    }

    close(serv_sock);
    return 0;
}

void error_handling(char *buf)
{
    fputs(buf, stderr);
    fputc('\n', stderr);
    exit(1);
}

