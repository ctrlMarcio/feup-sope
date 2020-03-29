#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXPATH 100
#define BASE_CLIENT "/tmp/fifo"
#define FIFO "/tmp/fifo.s"

int main(int argc, char **argv)
{
    pid_t pid = getpid();

    char path[MAXPATH];
    sprintf(path, "%s.%d", BASE_CLIENT, pid);

    mkfifo(path, 0777);

    int svfd = open(FIFO, O_WRONLY);
    int ownfd = open(path, O_RDONLY | O_NONBLOCK);

    char cmd[MAXPATH] = "";
    sprintf(cmd, "%d", pid);

    for (int i = 1; i < argc; ++i)
    {
        strcat(cmd, " ");
        strcat(cmd, argv[i]);
    }

    write(svfd, cmd, MAXPATH);

    char output[1000];
    read(ownfd, output, 1000);
    printf("%s\n", output);

    exit(0);
}