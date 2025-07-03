// 애코 서버: 클라이언트가 보낸 메시지를 다시 보내는 서버 (클라 -> 서버 -> 클라(양방향))
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024    // 버퍼 크기 정의

int main(int argc, char* argv[])
{
    int server_fd, client_fd;                       // 소켓 디스크립터
    struct sockaddr_in server_addr, client_addr;    // IPv4 관련 구조체
    socklen_t client_addr_size;                     // 클라이언트 주소 길이 저장용
    char buffer[BUFFER_SIZE];                       // 수송신 버퍼
    ssize_t bytes_read;                             // 읽어들인 바이트 수 저장용

    // 포트 번호 인자 없을 경우
    if(argc != 2) {
        printf("%s <port>\n", argv[0]);
        exit(1);
    }

    // 서버 소켓 생성
    server_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(server_fd == -1) {
        perror("socket failed");
        exit(1);
    }

    // 구조체 초기화
    memset(&server_addr, 0, sizeof(server_addr));   // 0으로 초기화
    server_addr.sin_family = AF_INET;               // IPv4 주소체계
    server_addr.sin_addr.s_addr = INADDR_ANY;       // 모든 IP
    server_addr.sin_port = htons(atoi(argv[1]));    // PORT 번호 지정 - 문자열 변환

    // 서버소켓과 주소 바인딩
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        close(server_fd);
        exit(1);
    }

    // 서버소켓 바인딩 - 클라이언트 연결 대기
    if(listen(server_fd, 5) == -1) {
        perror("listen failed");
        close(server_fd);
        exit(1);
    }

    // 클라이언트 소켓 생성 - 클라이언트 접속 수락
    client_addr_size = sizeof(client_addr);
    if((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_size)) == -1) {
        perror("accept failed");
        close(server_fd);
        exit(1);
    }   // 클라이언트로부터 메시지 받아 다시 돌려보냄 (echo): recv()로 읽고 send()로 돌려줌 반복
    while((bytes_read = recv(client_fd, buffer, sizeof(buffer)-1, 0)) != 0) {   // 읽은 바이트 수 0 아닌동안
        buffer[bytes_read] = '\0';  // 클라이언트에서 받은 데이터 buffer에 저장, 문자열 끝처리(널)
        printf("Message from client: %s", buffer);
        send(client_fd, buffer, bytes_read, 0);     // 받은 메시지 그대로 다시 보냄
    }

    close(client_fd);
    close(server_fd);

    return 0;
}
