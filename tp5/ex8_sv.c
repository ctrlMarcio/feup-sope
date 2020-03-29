#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define CHG "/tmp/fifo/chg"
#define MAXUSERNAME 20

int fd;
int count = 0;

void alarm_handler(int signo)
{
    printf("Total count: %d\n", count);
    exit(0);
}

int main(int argc, char **argv)
{
    int seconds = 20;

    if (argc > 1)
    {
        if ((seconds = atoi(argv[1])) == 0 || argc > 2)
        {
            printf("Usage: %s [sec]\n", argv[0]);
            exit(1);
        }
    }

    struct sigaction action;
    action.sa_handler = alarm_handler;
    action.sa_flags = 0;
    sigfillset(&action.sa_mask);

    sigaction(SIGALRM, &action, NULL);
    sigaction(SIGINT, &action, NULL);

    alarm(seconds);

    fd = open(CHG, O_RDONLY);
    if (fd == -1)
    {
        perror(CHG);
        exit(2);
    }

    char string[MAXUSERNAME];

    int tmp;
    while (1)
    {
        tmp = read(fd, string, MAXUSERNAME);
        if (tmp < 0)
        {
            perror("reading from fifo");
            exit(3);
        }
        else if (tmp)
        {
            printf("CHEGOU %s\n", string);
            count++;
        }
    }
    write(STDOUT_FILENO, "☣☣☣ WARNING ☣☣☣\nTHIS MESSAGE IS NOT SUPPOSED TO APPEAR\n", 56);
}