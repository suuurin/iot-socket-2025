#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>       // gethostbyname
#include <arpa/inet.h>   // inet_ntoa
#include <string.h>      // NULL

int main(int argc, char** argv)
{
    struct hostent *host;
    struct in_addr addr32;
    long int* addr;

    host = gethostbyname(argv[1]);
    if (host == NULL) {
        perror("error");
        exit(1);
    }

    printf("Official name: %s\n", host->h_name);
    printf("Addrtype: %d\n", host->h_addrtype);

    while (*host->h_addr_list != NULL) {
        addr = (long int*)host->h_addr_list;
        addr32.s_addr = *addr;
        printf("IP addr: %s\n", inet_ntoa(addr32));
        host->h_addr_list++;
    }

    return 0;
}
