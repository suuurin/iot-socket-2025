#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_size;
    char request[1024];

    char webpage[] = "HTTP/1.1 200 OK\r\n"
                     "Server:Linux Web Server\r\n"
                     "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                     "<!DOCTYPE html>\r\n"
                     "<html><head><title>My Web Page </title>\r\n"
                     "<style>body {background-color: #FFE4E1 }</style></head>\r\n"
                     "<body><center><h1>Hello world!</h1><br>\r\n"
                     "<img src=\"kirby.png\"></center></body></html>\r\n";

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(9090);

    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(serv_sock, 10);

    printf("Server started.");

    while (1) {
        clnt_size = sizeof(clnt_addr);
        int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_size);
        int len = read(clnt_sock, request, sizeof(request) - 1);
        request[len] = '\0';

        if (strstr(request, "GET /kirby.png") != NULL) {
            FILE* img = fopen("kirby.png", "rb");
            if (img) {
                char header[] =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: image/png\r\n\r\n";
                send(clnt_sock, header, strlen(header), 0);

                char buf[1024];
                int n;
                while ((n = fread(buf, 1, sizeof(buf), img)) > 0) {
                    send(clnt_sock, buf, n, 0);
                }
                fclose(img);
            } else {
                char not_found[] =
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: text/plain\r\n\r\n"
                    "Image not found.";
                send(clnt_sock, not_found, strlen(not_found), 0);
            }
        } else {
            send(clnt_sock, webpage, strlen(webpage), 0);
        }

        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}

