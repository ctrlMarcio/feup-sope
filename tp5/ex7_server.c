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
    if (mkfifo(REQUEST, 0777)) // error
    {
        perror("Client make request fifo");
        exit(1);
    }
    int reqfd = open(REQUEST, O_RDONLY);
    if (reqfd == -1)
    {
        perror("opening request");
        exit(2);
    }

    int ansfd;
    do
    {
        ansfd = open(ANSWER, O_WRONLY);

        if (ansfd == -1)
        {
            printf("Waiting for client ...\n");
            sleep(1);
        }
    } while (ansfd == -1);

    int a, b;

    read(reqfd, &a, MAX_LINE);
    read(reqfd, &b, MAX_LINE);

    while (a != 0 || b != 0)
    {
        int sum = a + b, difference = a - b, multiplication = a * b, mod = a % b;
        float division = ((float)a) / b;

        char type[MAX_LINE];

        write(ansfd, &sum, MAX_LINE);
        write(ansfd, &difference, MAX_LINE);
        write(ansfd, &multiplication, MAX_LINE);

        if (!b)
            strcpy(type, INVALID);
        else if (!mod) // then it is an integer
            strcpy(type, INTEGER);
        else
            strcpy(type, FLOAT);

        write(ansfd, type, MAX_LINE);
        write(ansfd, &division, MAX_LINE);

        read(reqfd, &a, MAX_LINE);
        read(reqfd, &b, MAX_LINE);
    }

    close(reqfd);
    close(ansfd);

    remove(REQUEST);
    remove(ANSWER);

    exit(0);
}