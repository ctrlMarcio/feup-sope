#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: %s <dir> <arg>\n", argv[0]);
        exit(1);
    }

    int firstpipe[2];
    if (pipe(firstpipe)) // error
    {
        perror("First pipe");
        exit(2);
    }

    pid_t firstchild;
    if ((firstchild = fork()) < 0) // error
    {
        perror("First fork");
        exit(3);
    }
    else if (firstchild > 0) // parent
    {
        close(firstpipe[READ]);

        dup2(firstpipe[WRITE], STDOUT_FILENO);

        execlp("ls", "ls", argv[1], "-laR", NULL);
        perror("Exec ls");
        exit(4);
    }
    else // first child
    {
        close(firstpipe[WRITE]);

        int secondpipe[2];
        if (pipe(secondpipe)) // error
        {
            perror("Second pipe");
            exit(2);
        }

        pid_t secondchild;
        if ((secondchild = fork()) < 0) // error
        {
            perror("Second fork");
            exit(3);
        }
        else if (secondchild > 0) // parent
        {
            close(secondpipe[READ]);

            dup2(firstpipe[READ], STDIN_FILENO);
            dup2(secondpipe[WRITE], STDOUT_FILENO);

            execlp("grep", "grep", argv[2], NULL);
            perror("Exec grep");
            exit(4);
        }
        else // child
        {
            close(secondpipe[WRITE]);

            dup2(secondpipe[READ], STDIN_FILENO);

            execlp("sort", "sort", NULL);
            perror("Exec sort");
            exit(4);
        }
    }

    exit(0);
}