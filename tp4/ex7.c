#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc == 1) {
        printf("Usage: %s <programs ...>\n", argv[0]);
        exit(1);
    }

    // creates all subprocesses and runs them
    pid_t pid;
    for (int i = 0; i < argc - 1; ++i) {
        pid = fork();

        if (pid == 0)
            execlp(argv[i + 1], argv[i + 1], NULL);
    }

    // waits for the processes
    int status, termination;
    for (int i = 0; i < argc - 1; ++i) {
        wait(&status);
        termination = WTERMSIG(status);
        printf("Termination status: %d\n", termination);

        if (termination != 0)
            kill(0, SIGKILL); // kills all children 
    }
}