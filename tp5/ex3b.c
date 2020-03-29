#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    int pipefd[2];

    if (pipe(pipefd) < 0)
    {
        perror("Pipe");
        exit(2);
    }

    pid_t childpid;

    if ((childpid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }
    if (childpid > 0) // parent
    {
        close(pipefd[READ]);
        dup2(pipefd[WRITE], STDOUT_FILENO);
        usleep(500); // safety net
        execlp("cat", "cat", argv[1], NULL);
        perror("execlp cat");
        exit(3);
    }
    else if (childpid == 0) // child
    {
        
        
        close(pipefd[WRITE]);
        dup2(pipefd[READ], STDIN_FILENO);

        execlp("sort", "sort", NULL);
        perror("execlp sort");
        exit(4);
    }

    exit(0);
}