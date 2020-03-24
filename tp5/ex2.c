#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1
#define MAX_LINE 10
#define INVALID "i"
#define INTEGER "d"
#define FLOAT "f"

int main()
{
    int parent_child[2];
    int child_parent[2];
    pipe(parent_child);
    pipe(child_parent);

    pid_t pid = fork();

    if (pid > 0)
    { // parent
        close(parent_child[READ]);
        close(child_parent[WRITE]);

        int a, b;
        scanf("%d %d", &a, &b);

        write(parent_child[WRITE], &a, MAX_LINE);
        write(parent_child[WRITE], &b, MAX_LINE);

        close(parent_child[WRITE]);

        int sum, difference, multiplication;
        float division;
        char type[MAX_LINE];

        read(child_parent[READ], &sum, MAX_LINE);
        read(child_parent[READ], &difference, MAX_LINE);
        read(child_parent[READ], &multiplication, MAX_LINE);
        read(child_parent[READ], type, MAX_LINE);
        read(child_parent[READ], &division, MAX_LINE);

        printf("%-10d: Sum\n", sum);
        printf("%-10d: Difference\n", difference);
        printf("%-10d: Multiplication\n", multiplication);
        if (!strcmp(type, INVALID))
            printf("Division by 0 not possible\n");
        else if (!strcmp(type, INTEGER))
            printf("%-10d: Division\n", (int) division);
        else if (!strcmp(type, FLOAT))
            printf("%-10f: Division\n", division);

        close(child_parent[READ]);

        wait(NULL);
    }
    else if (pid == 0)
    { // child
        close(parent_child[WRITE]);
        close(child_parent[READ]);

        int a, b;

        read(parent_child[READ], &a, MAX_LINE);
        read(parent_child[READ], &b, MAX_LINE);

        close(parent_child[READ]);

        int sum = a + b, difference = a - b, multiplication = a * b, mod = a % b;
        float division = ((float)a) / b;

        char type[MAX_LINE];

        write(child_parent[WRITE], &sum, MAX_LINE);
        write(child_parent[WRITE], &difference, MAX_LINE);
        write(child_parent[WRITE], &multiplication, MAX_LINE);

        if (!b)
            strcpy(type, INVALID);
        else if (!mod) // then it is an integer
            strcpy(type, INTEGER);
        else
            strcpy(type, FLOAT);

        write(child_parent[WRITE], type, MAX_LINE);
        write(child_parent[WRITE], &division, MAX_LINE);

        close(child_parent[WRITE]);
    }
}