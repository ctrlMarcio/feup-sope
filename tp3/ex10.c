#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[], char *envp[])
{
    pid_t pid;
    int status;

    if (argc != 2 && argc != 3)
    {
        printf("usage: %s dirname [saving_dirname]\n", argv[0]);
        exit(1);
    }

    pid = fork();

    if (pid > 0)
    {
        printf("My child is going to execute command \"ls -laR %s\"\n", argv[1]);
        wait(&status);
        printf("Child exit code: %d\n", status);
    }
    else if (pid == 0)
    {
        if (argc == 3) {
            int fd = open(argv[2], O_WRONLY | O_CREAT | O_EXCL);

            if (fd == -1) {
                perror(argv[2]);
                exit(2);
            }

            dup2(fd, STDOUT_FILENO);
        }
        
        execlp("ls", "ls", "-laR", argv[1], NULL); // TODO: is the file closed?
        printf("Command not executed !\n");
        exit(1);
    }
    exit(0);
}