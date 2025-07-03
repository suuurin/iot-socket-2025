// 문자열인 IP 주소와 포트번호를 변환해 네트워크용 주소 구조체에 저장하기
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <stdlib.h>

// IPv4 주소 구조체 (in_addr)
struct myin_addr {
    uint32_t s_addr;    // 32비트 IP 주소
};

// IPv4 소켓 주소 구조체 (sockaddr_in)
struct mysockaddr_in {
    unsigned short sin_family;
    struct myin_addr sin_addr;  // ip 주소
    uint16_t sin_port;          // 포트번호
};

void main()
{
    struct mysockaddr_in myaddr;    // 구조체 변수
    const char* serv_ip = "127.0.0.1";  // 문자열 ip
    const char* serv_port = "8080";     // 문자열 포트번호

    // 구조체 0으로 초기화
    memset(&myaddr, 0, sizeof(myaddr));

    // 주소 체계 설정(IPv4)
    myaddr.sin_family = PF_INET;

    // IP 주소용 함수 inet_addr로 변환: 0x7F000001
    myaddr.sin_addr.s_addr = inet_addr(serv_ip);

    // 포트번호 문자열: "8080" -> 정수 변환 atoi(): 8080 -> 네트워크 바이트 변환 htons(): 0x901f
    myaddr.sin_port = htons(atoi(serv_port));

    // 결과 출력
    printf("sin_family: %d\n", myaddr.sin_family);
    printf("sin_addr: %#\n", myaddr.sin_addr.s_addr);
    printf("sin_port: %#x\n", myaddr.sin_port);
}

/*
    AF INET (주소체계) = PF_INET(프로토콜패밀리)
*/
