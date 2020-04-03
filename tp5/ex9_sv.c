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
#define FIFO "/tmp/fifo.s"      // the main fifo
#define BASE_CLIENT "/tmp/fifo" // client fifo name /tmp/fifo.<pid>
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
    // creates the fifo if it doesn't exist
    if (access(FIFO, 0777))
    {
        mkfifo(FIFO, 0777);
    }

    // opens the fifo and the auxiliary file
    int fd = open(FIFO, O_RDONLY);
    int out = open(OUT, O_WRONLY | O_CREAT);

    char string[MAXCMD];
    int tmp;
    while (1) // in an usual application, there would be an escape somewhere
    {
        // reads from the client
        tmp = read(fd, string, MAXCMD);
        if (tmp < 0)
        {
            perror("reading from fifo");
            exit(2);
        }
        else if (tmp)
        {
            // breaks the command into an array
            char *cmds[MAXARGS];
            int size = split(string, " ", cmds);

            cmds[size] = 0; // the array is terminated with \0

            // builds the client fifo name's string
            char client_fifo_name[30];
            sprintf(client_fifo_name, "%s.%s", BASE_CLIENT, cmds[0]);

            int clientfifo = open(client_fifo_name, O_WRONLY);

            if (size < 2)
            {
                write(clientfifo, "Invalid command", MAXLINE);
                continue;
            }

            // multiprocessing to execute the comand
            pid_t pid = fork();

            if (pid < 0)
            {
                perror("fork");
                exit(3);
            }
            else if (pid == 0)
            {
                // child executes the command
                dup2(out, STDOUT_FILENO);
                execvp(cmds[1], &(cmds[1]));

                perror(cmds[1]);
                exit(4);
            }
            else
            {
                // parent
                close(out);

                wait(NULL);

                // reads the file written by the child
                char buffer[MAXLINE] = "";
                char line[MAXCMD];
                FILE *fp = fopen(OUT, "r");
                while (fgets(line, sizeof(line), fp))
                    strcat(buffer, line);
                fclose(fp);
                // writes it in the pipe
                if (write(clientfifo, buffer, MAXLINE) > 0)
                    printf("ran %s\n", cmds[1]);
            }
        }
    }

    close(fd);

    exit(0);
}