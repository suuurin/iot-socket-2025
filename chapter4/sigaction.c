#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
    if(sig == SIGALRM) puts("timeout");
    alarm(2);
}

int main()
{
    struct sigaction act;
    act.sa_handler = timeout;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM, &act, 0);

    alarm(2);

    for(int i = 0; i < 3; i++) {
        puts("wait...");
        sleep(100);
    }

    return 0;
}
