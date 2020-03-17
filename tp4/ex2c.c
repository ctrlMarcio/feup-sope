#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sigint_handler(int signo)
{
    printf("Entering SIGINT handler ...\n");
    sleep(10);
    printf("Exiting SIGINT handler ...\n");
}

void sigterm_handler(int signo)
{
    printf("Entering SIGTERM handler ...\n");
    printf("Exiting SIGTERM handler ...\n");
    exit(0);
}

int main(void)
{
    struct sigaction intaction;
    intaction.sa_handler = sigint_handler;
    sigemptyset(&intaction.sa_mask);
    intaction.sa_flags = 0;

    if (sigaction(SIGINT, &intaction, NULL) < 0)
    {
        fprintf(stderr, "Unable to install SIGINT handler\n");
        exit(1);
    }

    struct sigaction termaction;
    termaction.sa_handler = sigterm_handler;
    sigemptyset(&termaction.sa_mask);
    termaction.sa_flags = 0;

    if (sigaction(SIGTERM, &termaction, NULL) < 0)
    {
        fprintf(stderr, "Unable to install SIGTERM handler\n");
        exit(1);
    }

    printf("Try me with CTRL-C ...\n");
    while (1)
        pause();
    exit(0);
}