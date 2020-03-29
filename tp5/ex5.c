/**
 * NOT WORKING, WILL CHECK LATER
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX 100
#define MAXCMDS 10
#define DELIMITOR "|"
#define READ 0
#define WRITE 1

int split(char *src, char *delimitor, char *dest[]);
int trim_string(char *out, const char *str);
void runcmds(char **cmds, int size, int i, int fd[]);

int main()
{
    char cmd[MAX];

    write(STDOUT_FILENO, "> ", 3);
    read(STDIN_FILENO, cmd, MAX);

    char *vcmd[MAXCMDS];
    int size = split(cmd, DELIMITOR, vcmd);

    for (int i = 0; i < size; ++i)
        trim_string(vcmd[i], vcmd[i]);

    int oldfd[2];

    if (pipe(oldfd)) // error
    {
        perror("pipe");
        exit(1);
    }

    runcmds(vcmd, size, 0, oldfd);

    exit(0);
}

int split(char *src, char *delimitor, char *dest[])
{
    int count = 0;

    char *token = strtok(src, delimitor);
    dest[count++] = token;

    while ((token = strtok(NULL, delimitor)))
        dest[count++] = token;

    return count;
}

// inspired by
// https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
int trim_string(char *out, const char *str)
{
    int len = strlen(str);
    if (len == 0)
        return 0;

    const char *end;
    size_t out_size;

    // Trim leading space
    while (*str == ' ' || *str == '\n')
        str++;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\n'))
        end--;
    end++;

    // Set output size to minimum of trimmed string length and buffer size minus 1
    out_size = (end - str) < len - 1 ? (end - str) : len - 1;

    // Copy trimmed string and add null terminator
    memcpy(out, str, out_size);
    out[out_size] = 0;

    return out_size;
}

void runcmds(char **cmds, int size, int i, int fd[])
{
    if (i == size) {
        dup2(STDOUT_FILENO, fd[WRITE]);
        return;
    }

    dup2(fd[READ], STDIN_FILENO);

    pid_t pid;
    if ((pid = fork()) < 0) // error
    {
        perror("fork");
        exit(2);
    }
    else if (pid > 0) // parent
    {
        close(fd[READ]);

        char *singlecmd[MAXCMDS];
        split(cmds[i], " ", singlecmd);

        dup2(fd[WRITE], STDOUT_FILENO);

        execvp(singlecmd[0], singlecmd);
        perror(singlecmd[0]);
        exit(3);
    }
    else // child
    {
        close(fd[WRITE]);

        int newfd[2];

        if (pipe(newfd)) // error
        {
            perror("pipe");
            exit(1);
        }

        close(newfd[READ]);

        runcmds(cmds, size, i++, newfd);
    }
}
