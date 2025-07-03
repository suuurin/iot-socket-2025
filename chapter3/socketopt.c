#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

int main() 
{
    int sock;
    int sndbuf, revbuf;
    socketlen_t opt1en;

    sock = socket(PF_INET, SOCK_STREAM, 0);

    optlen = sizeof(sndbuf);
    if(getsockopt(sock, SOL_SOCKET, SO_SNDBUP, (void*)&sndbuf, &optlen) < 0)
        perror("getsockopt failed");

    printf("sendbuf size: %d\n", sndbuf);



    return 0;
}
