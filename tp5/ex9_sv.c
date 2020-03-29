/* INCOMPLETE FOR NOW, WILL COME BACK LATER 😬*/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define OUT "./aux"
#define FIFO "/tmp/fifo.s"
#define BASE_CLIENT "/tmp/fifo"
#define MAXCMD 30
#define MAXARGS 10
#define MAXLINE 1000

int split(char *src, char *delimitor, char *dest[])
{
    int count = 0;

    char *token = strtok(src, delimitor);
    dest[count++] = token;

    while ((token = strtok(NULL, delimitor)))
        dest[count++] = token;

    return count;
}

int main()
{
    if (access(FIFO, 0777))
    {
        mkfifo(FIFO, 0777);
    }

    int fd = open(FIFO, O_RDONLY);
    int out = open(OUT, O_WRONLY | O_CREAT | O_EXCL);

    char string[MAXCMD];
    int tmp;
    while (1)
    {
        tmp = read(fd, string, MAXCMD);
        if (tmp < 0)
        {
            perror("reading from fifo");
            exit(2);
        }
        else if (tmp)
        {
            char *cmds[MAXARGS];
            int size = split(string, " ", cmds);

            cmds[size] = 0;

            char client_fifo_name[30];
            sprintf(client_fifo_name, "%s.%s", BASE_CLIENT, cmds[0]);

            int clientfifo = open(client_fifo_name, O_WRONLY);

            pid_t pid = fork();

            if (pid < 0)
            {
                perror("fork");
                exit(3);
            }
            else if (pid == 0) // child executes
            {
                dup2(out, STDOUT_FILENO);
                execvp(cmds[1], &(cmds[1]));

                perror(cmds[1]);
                exit(4);
            }
            else // parent
            {
                wait(NULL);

                FILE *fp = fdopen(fd, "r");

                char tmp[MAXLINE];
                while (fgets(tmp, MAXLINE, fp))
                    write(clientfifo, tmp, MAXLINE);
            }
        }
    }

    close(fd);

    exit(0);
}