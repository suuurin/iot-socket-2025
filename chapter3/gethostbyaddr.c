//IP 주소 입력 받아 해당 IP의 도메인(호스트) 이름 출력
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char** argv)
{
    struct hostent *host;
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    inet_aton(argv[1], &addr.sin_addr);

    // gethostnyaddr(): 역방향 DNS 조회 함수
    host = gethostbyaddr(&addr.sin_addr, sizeof(addr.sin_addr), AF_INET);
    printf("Hostname: %s\n", host->h_name);

    return 0;
}
