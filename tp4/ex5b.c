#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void child_hndl(int signo) {
    wait(NULL);
}

int main(void)
{
    pid_t pid;
    int i, n;

    for (i = 1; i <= 10; i++)
    {
        pid = fork();
        if (pid == 0)
        { // child
            printf("CHILD no. %d (PID=%d) working ... \n", i, getpid());
            sleep(15); // child working ...
            printf("CHILD no. %d (PID=%d) exiting ... \n", i, getpid());
            exit(0);
        }
    }

    struct sigaction action;
    action.sa_handler = child_hndl;
    action.sa_flags = 0;

    sigaction(SIGCHLD, &action, NULL);

    for (i = 1; i <= 4; i++)
    {
        printf("PARENT: working hard (task no. %d) ...\n", i);
        n = 20;
        while ((n = sleep(n)) != 0);
        printf("PARENT: end of task no. %d\n", i);
        printf("PARENT: waiting for child no. %d ...\n", i);
    }
    exit(0);
}