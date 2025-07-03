#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdint.h>

// IPv4 주소 구조체
struct myin_addr {
    uint32_t s_addr;    // ip 주소 저장할  32비트 필드
};

// IPv4 소켓 주소 구조체
struct mysockaddr_in {
    unsigned short sin_family;  // 주소체계
    struct myin_addr sin_addr;  // ip 주소
    uint16_t sin_port;          // 포트 번호
};

int main()
{
    // 변수 선언
    struct mysockaddr_in myaddr;

    // 구조체 전체 필드를  0으로 초기화
    memset(&myaddr, 0, sizeof(myaddr));

    // 필드 값 출력
    printf("sin_family: %d\n", myaddr.sin_family);
    printf("sin_addr: %d\n", myaddr.sin_addr.s_addr);
    printf("sin_port: %d\n", myaddr.sin_port);
}
