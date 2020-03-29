#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1
#define MAX_LINE 10

int main()
{
    int fd[2];
    pipe(fd);

    pid_t pid = fork();

    if (pid > 0)
    { // parent
        close(fd[READ]);

        char a[MAX_LINE + 1], b[MAX_LINE];
        scanf("%s %s", a, b);

       //strcat(a, " ");
        write(fd[WRITE], a, MAX_LINE);
        write(fd[WRITE], b, MAX_LINE);

        close(fd[WRITE]);
        wait(NULL);
    }
    else if (pid == 0)
    { // child
        close(fd[WRITE]);

        char a[MAX_LINE];
        char b[MAX_LINE];

        read(fd[READ], a, MAX_LINE);
        read(fd[READ], b, MAX_LINE);

        int aa = atoi(a);
        int bb = atoi(b);

        printf("%-10d: Sum\n", aa + bb);
        printf("%-10d: Difference\n", aa - bb);
        printf("%-10d: Multiplication\n", aa * bb);
        if (b != 0)
            printf("%-10f: Division\n", ((float)aa) / bb);
        else
            printf("Division by 0 not possible\n");

        close(fd[READ]);
    }
}