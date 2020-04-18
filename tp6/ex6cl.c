#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define REQUEST "/tmp/fifo_req"
const char *ANSWER = "/tmp/fifo_ans_";

struct operation
{
    int a, b;
    char op;
    char fifo[30];
} operation;

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

    char answer[100];
    sprintf(answer, "%s%d", ANSWER, getpid());

    if (mkfifo(answer, 0777)) // error
    {
        perror("Client make request fifo");
        exit(1);
    }

    struct operation operation;

    printf("The operation pretty please (a<op>b) ");
    scanf("%d%c%d", &(operation.a), &(operation.op), &(operation.b));

    if (operation.op != '+' && operation.op != '-' && operation.op != '*' && operation.op != '/')
    {
        fprintf(stderr, "Invalid operation\n");
        remove(answer);
        exit(0);
    }

    strcpy(operation.fifo, answer);

    write(req, &operation, sizeof(struct operation));

    if (operation.a != 0 && operation.b != 0)
    {
        int ans = open(answer, O_RDONLY);

        double res;

        read(ans, &res, sizeof(double));

        if (operation.op == '/')
            printf("res: %.2f\n", res);
        else
            printf("res: %d\n", (int)res);

        close(ans);
    }
    else
    {
        printf("Server terminated\n");
    }

    remove(answer);
}