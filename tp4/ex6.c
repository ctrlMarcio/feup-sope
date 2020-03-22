#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

pid_t pid = 0;

void alarmHandler(int signo) {
    kill(pid, SIGKILL);
}

int main(int argc, char *argv[])
{
    int t;
    if (argc < 3 || (t = atoi(argv[1])) == 0) {
        printf("Usage: %s <int> <program>\n", argv[0]);
        exit(1);
    }

    pid = fork();

    if (pid == 0)
    {
        execv(argv[2], &argv[2]);
        printf("Error executing %s", argv[2]);
        exit(1);
    }
    else
    {
        struct sigaction action;
        action.sa_handler = alarmHandler;
        action.sa_flags = 0;
        sigfillset(&action.sa_mask);

        sigaction(SIGALRM, &action, NULL);

        alarm(t);
    }

    int status;
    wait(&status);  
    if (WIFEXITED(status))
        printf("Child ended normally\n");
    else
        printf("Child ended abnormally\n");

    exit(0);
}