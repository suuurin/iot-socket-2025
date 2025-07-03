// 파일 전송 클라이언트
// 서버에 접속해 파일 내용을 받고 로컬 파일로 저장 후 감사 메일 전송
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30     // 한 번에 수신 받을 최대 바이트수

int main(int argc, char** argv)
{
    int serv_fd;            // 서버와 연결될 소켓 디스크립터
    FILE* fp;               // 수신 데이터 저장할 파일 포인터
    char buf[BUF_SIZE];     // 수신 버퍼
    int read_cnt;           // 실제로 받은(읽은) 바이트 수

    struct sockaddr_in serv_addr;   // 서버 주소 구조체

    // 인자 갯수 3개인지 확인
    if(argc != 3) {
        printf("사용법: %s <IP> <PORT>\n", argv[0]);
        exit(1);
    }

    // 저장할 파일 열기 (쓰기 모드,  wb?)
    fp = fopen("receive.dat", "wb");
    serv_fd = socket(PF_INET, SOCK_STREAM, 0);  // 소켓 생성

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // 서버에 연결 요청
    connect(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // 서버로부터 데이터 수신 및 파일에 저장
    while((read_cnt = read(serv_fd, buf, BUF_SIZE)) != 0)
        fwrite((void*)buf, 1, read_cnt, fp);    // 받은 내용 그대로 파일에 씀

    puts("Received file data");     // 수신 완료 메시지 출력
    write(serv_fd, "Thank you", 10);    // 서버에 감사 메시지 전송

    // 종료
    fclose(fp);
    close(serv_fd);

    return 0;
}
