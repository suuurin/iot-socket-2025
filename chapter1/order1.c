#include <stdio.h>
#include <arpa/inet.h>

void main()
{
    unsigned short port = 0x1234;
    unsigned int ip = 0x12345678;

    unsigned short network_port;
    unsigned int network_ip;

    network_port = htons(port);
    network_ip = htonl(ip);

    printf("ip: %#x", ip);
    printf(" port: %#x\n", port);
    printf("conversion ip: %#x", network_ip);
    printf(" conversion port: %#x\n", network_port);
}
