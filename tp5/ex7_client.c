#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define REQUEST "/tmp/fifo_req"
#define ANSWER "/tmp/fifo_ans"
#define MAX_LINE 10
#define INVALID "i"
#define INTEGER "d"
#define FLOAT "f"

int main()
{
    int req;
    do
    {
        req = open(REQUEST, O_WRONLY);
        if (req == -1)
        {
            printf("Waiting for server ...\n");
            sleep(1);
        }
    } while (req == -1);

    if (mkfifo(ANSWER, 0777)) // error
    {
        perror("Client make request fifo");
        exit(1);
    }

    int a, b;
    write(STDOUT_FILENO, "Numbers <a b>: ", 15);
    scanf("%d %d", &a, &b);

    write(req, &a, MAX_LINE);
    write(req, &b, MAX_LINE);

    a = 0;
    b = 0;
    write(req, &a, MAX_LINE);
    write(req, &b, MAX_LINE);

    close(req);

    int ans = open(ANSWER, O_RDONLY);

    int sum, difference, multiplication;
    float division;
    char type[MAX_LINE];

    read(ans, &sum, MAX_LINE);
    read(ans, &difference, MAX_LINE);
    read(ans, &multiplication, MAX_LINE);
    read(ans, type, MAX_LINE);
    read(ans, &division, MAX_LINE);

    printf("%-10d: Sum\n", sum);
    printf("%-10d: Difference\n", difference);
    printf("%-10d: Multiplication\n", multiplication);
    if (!strcmp(type, INVALID))
        printf("Division by 0 not possible\n");
    else if (!strcmp(type, INTEGER))
        printf("%-10d: Division\n", (int)division);
    else if (!strcmp(type, FLOAT))
        printf("%-10f: Division\n", division);

    close(ans);

    exit(0);
}