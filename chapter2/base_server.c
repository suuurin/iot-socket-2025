// TCP 서버 기본 구조 : 클라이언트 연결되면 Hello World 전송
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
    int server_fd, client_fd;                       // 서버/클라 소켓 파일 디스크럽터
    struct sockaddr_in server_addr, client_addr;    // 서버/클라 주소 구조체
    socklen_t client_addr_len;                      // 클라이언트 주소 구조체 크기
    const char* message = "Hello world!\n";         // 클라이언트에 보낼 메시지

    // 1. 서버 소켓 생성 (PF_INET = IPv4, SOCK_STREAM = TCP)
    if((server_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(1);
    }

    // 2. 서버 주소 구조체 초기화
    memset(&server_addr, 0, sizeof(server_addr));   // 0으로 초기화 
    server_addr.sin_family = AF_INET;               // IPv4 사용
    server_addr.sin_addr.s_addr = INADDR_ANY;       // 모든 IP에서 접근 허용 (s_addr: IP주소값)
    server_addr.sin_port = htons(atoi(argv[1]));    // 문자열 네트워크 바이트 변환

    // 3. 소켓에 IP와 포트 정보 바인딩
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        close(server_fd);
        exit(1);
    }

    // 4. 연결 요청 대기 (클라 연결 기다림)
    if(listen(server_fd, 5) == -1) {
        perror("listen failed");
        close(server_fd);
        exit(1);
    }

    // 5. 클라이언트 연결 수락
    client_addr_len = sizeof(client_addr);
    if((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len)) == -1) {
        perror("accept failed");
        close(server_fd);
        exit(1);
    }

    // 6. 클라이언트에 메시지 전송
    if(write(client_fd, message, strlen(message)) == -1)
        perror("write failed");

    // 7. 소켓 종료
    close(client_fd);
    close(server_fd);

    return 0;
}
