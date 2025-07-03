// 파일 전송 서버
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30 // 한 번에 전송할 바이트 수

int main(int argc, char** argv)
{
    int serv_fd, clnt_fd;
    FILE* fp;               // 전송할 파일 포인터
    char buf[BUF_SIZE];     // 전송할 버퍼
    int read_cnt;           // fread로 읽은 바이트 수

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    // 포트번호 미입력시
    if (argc != 2) {
        printf("사용법: %s <port>\n", argv[0]);
        exit(1);
    }

    // 전송할 파일 열기
    fp = fopen("file_server.c", "rb");  // fopen 파일 열기 자기 자신
    if (fp == NULL) {
        perror("파일 열기 실패");
        exit(1);
    }

    // 서버 소켓 생성
    serv_fd = socket(PF_INET, SOCK_STREAM, 0);

    // 서버 주소 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 바인딩 및 연결 대기
    bind(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(serv_fd, 5); // 최대 5명까지 대기

    // 클라이언트 접속 수락
    clnt_addr_sz = sizeof(clnt_addr);
    clnt_fd = accept(serv_fd, (struct sockaddr*)&clnt_addr, &clnt_addr_sz); 

    // 파일 내용 전송
    while (1) {
        // 파일에서 BUF_SIZE만큼 읽기
        read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
        if (read_cnt < BUF_SIZE)  {             // 마지막 전송이면
            write(clnt_fd, buf, read_cnt);      // 남은 데이터만큼 전송
            break;
        }
        write(clnt_fd, buf, BUF_SIZE);          // 일반 전송
    }

    // 전송 완료 알림, 쓰기 종료
    shutdown(clnt_fd, SHUT_WR);

    // 클라이언트로부터 메시지 수신 (ex: "Thank you")
    read(clnt_fd, buf, BUF_SIZE);
    printf("message from client: %s\n", buf);

    // 종료
    fclose(fp);
    close(clnt_fd);
    close(serv_fd);

    return 0;
}
