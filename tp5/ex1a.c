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

        int a, b;
        scanf("%d %d", &a, &b);

        write(fd[WRITE], &a, MAX_LINE);
        write(fd[WRITE], &b, MAX_LINE);

        close(fd[WRITE]);
        wait(NULL);
    }
    else if (pid == 0)
    { // child
        close(fd[WRITE]);

        int a, b;
        
        read(fd[READ], &a, MAX_LINE);
        read(fd[READ], &b, MAX_LINE);

        printf("%-10d: Sum\n", a + b);
        printf("%-10d: Difference\n", a - b);
        printf("%-10d: Multiplication\n", a * b);
        if (b != 0)
            printf("%-10f: Division\n", ((float)a) / b);
        else
            printf("Division by 0 not possible\n");

        close(fd[READ]);
    }
}