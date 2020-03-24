#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1
#define MAX_LINE 4096

struct operands
{
    int a;
    int b;
};

int main()
{
    int fd[2];
    pipe(fd);

    pid_t pid = fork();

    if (pid > 0)
    { // parent
        close(fd[READ]);

        struct operands operands;

        scanf("%d %d", &operands.a, &operands.b);

        write(fd[WRITE], &operands, MAX_LINE);

        close(fd[WRITE]);
        wait(NULL);
    }
    else if (pid == 0)
    { // child
        close(fd[WRITE]);

        struct operands operands;

        read(fd[READ], &operands, MAX_LINE);
        int a = operands.a;
        int b = operands.b;

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