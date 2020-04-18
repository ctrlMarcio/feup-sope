#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define REQUEST "/tmp/fifo_req"

struct operation
{
    int a, b;
    char op;
    char fifo[30];
} operation;

void *treat_client(void *arg)
{
    struct operation *operation = (struct operation *) arg;
    int a = operation->a, b = operation->b;

    double res;
    if (operation->op == '+')
        res = a + b;
    else if (operation->op == '-')
        res = a - b;
    else if (operation->op == '*')
        res = a * b;
    else if (operation->op == '/')
        res = ((double)a) / b;

    int ansfd;
    do
    {
        ansfd = open(operation->fifo, O_WRONLY);
    } while (ansfd == -1);

    write(ansfd, &res, sizeof(double));

    close(ansfd);

    return NULL;
}

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
        fprintf(stderr, "opening request");
        exit(2);
    }


    struct operation operation;

    read(reqfd, &operation, sizeof(struct operation));

    pthread_t p;
    while (operation.a != 0 && operation.b != 0)
    {
        if (pthread_create(&p, NULL, treat_client, &operation)) {
            perror("Error creating a thread\n");
            exit(0);
        }

        while (read(reqfd, &operation, sizeof(struct operation)) <= 0);
        printf("askdnasd\n");
    }

    close(reqfd);
    remove(REQUEST);

    exit(0);
}