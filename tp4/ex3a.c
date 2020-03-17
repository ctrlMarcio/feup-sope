#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int v = 0;
char raising = 1;

void handler(int signo) {
    if (signo == SIGUSR1)
        raising = 1;
    if (signo == SIGUSR2)
        raising = 0;       
}

int main() {
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);

    char string[50];

    while (1) {
        sprintf(string, "%d\n", v);
        write(STDOUT_FILENO, string,strlen(string));

        sleep(1);

        if (raising)
            v++;
        else
            v--;
    }

    exit(0);
}