#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100

// 자식 프로세스 종료 시 시그널 핸들러
void read_childproc(int sig)
{
    int status;
    waitpid(-1, &status, WNOHANG);  // 좀비 프로세스 방지용
}

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    int fds[2];  // 파이프: [0] 읽기, [1] 쓰기

    pid_t pid;
    struct sigaction act;
    socklen_t adr_sz;
    int str_len, state;
    char buf[BUF_SIZE];

    // 포트번호 확인
    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 시그널 핸들러 설정 (자식 종료 처리)
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);

    // 서버 소켓 생성 및 설정
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind() error"); close(serv_sock); exit(1);
    }

    if (listen(serv_sock, 5) == -1) {
        perror("listen() error"); close(serv_sock); exit(1);
    }

    pipe(fds);      // 부모-자식 간 파이프 생성
    pid = fork();   // 자식 생성 (파일 기록용)

    if (pid == 0) {
        // 자식 프로세스: 파이프에서 읽어서 파일에 저장
        FILE *fp = fopen("echomsg.txt", "wt");
        char msgbuf[BUF_SIZE];
        int i, len;

        for (i = 0; i < 10; i++) {
            len = read(fds[0], msgbuf, BUF_SIZE);   // 파이프 읽기
            fwrite((void*)msgbuf, 1, len, fp);      // 파일 기록
        }

        fclose(fp);
        return 0;
    }

    // 부모 프로세스: 클라이언트 처리 루프
    while (1) {
        adr_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz);
        if (clnt_sock == -1) continue;

        if ((pid = fork()) == 0) {
            // 자식 프로세스: 클라이언트 통신 처리
            close(serv_sock);  // 자식은 서버 소켓 닫기

            while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0) {
                write(clnt_sock, buf, str_len);  // 클라이언트에게 에코
                write(fds[1], buf, str_len);     // 파이프에 메시지 저장
            }

            close(clnt_sock);
            return 0;
        } else {
            // 부모 프로세스: 클라이언트 소켓 닫기 (자식이 처리하므로)
            close(clnt_sock);
        }
    }

    close(serv_sock);  // 서버 종료 시 소켓 닫기
    return 0;
}
