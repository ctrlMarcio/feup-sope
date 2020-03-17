#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>

const int TIMES = 5;
int v = 0;
int counter = 0;

void handler(int signo)
{
    if (signo == SIGUSR1)
        v++;
    if (signo == SIGUSR2)
        v--;

    char string[50];
    sprintf(string, "%d\n", v);
    write(STDOUT_FILENO, string, strlen(string));
    counter++;
}

int main()
{
    srand(time(NULL));

    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);

    pid_t pid = fork();

    if (pid > 0)
    {
        for (int i = 0; i < TIMES; ++i)
        {
            int rando = (int)rand() % 2;
            int signo = rando ? SIGUSR1 : SIGUSR2;

            kill(pid, signo);

            sleep(1);
        }
    }

    if (pid == 0) 
    {
        while (counter < TIMES) {
            pause();
        }
    }
    if (pid > 0)
        wait(NULL);

    exit(0);
}