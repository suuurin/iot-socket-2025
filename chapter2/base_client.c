// TCP 클라이언트 기본 구조 - 서버가 보내는 메시지 받아 출력
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
    int sock_fd;                        // 클라이언트 소켓 파일 디스크립터
    struct sockaddr_in server_addr;     // 서버 주소 정보 구조체
    char buf[1024];                     // 수신 버퍼
    int recv_len;                       // 수신된 바이트 수

    // 소켓 생성(IPv4, TCP) : socket(domain, type, protocol)
    if((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    } // 소켓 생성 실패하는 이유? 소켓 한도 초과, 메모리 부족, 잘못된 인자(3가지중) ...

    // 서버 주소 구조체 초기화 및 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);   // IP 문자열 변환
    server_addr.sin_port = htons(atoi(argv[2]));        // 포트 문자열 변환

    // 서버에 연결 시도 (connect 호출), 실패시 예외처리
    if(connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        close(sock_fd);
        exit(1);
    }

    // 서버로부터 메시지 수신, ..
    recv_len = recv(sock_fd, buf, sizeof(buf) - 1, 0);  // recv: receive
    if(recv_len == -1) printf("receive failed");

    printf("Message from server: %s %ld %s\n", buf, sizeof(buf), buf[14]);

    // 소켓 닫기
    close(sock_fd);

    return 0;
}
