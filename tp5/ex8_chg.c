#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#define CHG "/tmp/fifo/chg"
#define MAXUSERNAME 20

int main(int argc, char **argv)
{
    /* TESTING
    if (mkfifo(CHG, 0777)) // error
    {
        perror("mkfifo");
        exit(1);
    }
    */

    if (argc != 2)
    {
        printf("Usage: %s <username>\n", argv[0]);
        exit(1);
    }

    int fd = open(CHG, O_WRONLY);
    if (fd == -1)
    {
        perror(CHG);
        exit(2);
    }

    write(fd, argv[1], MAXUSERNAME);

    close(fd);

    exit(0);
}