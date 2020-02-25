#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

const int MAX_SIZE = 10000;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        write(STDOUT_FILENO, "Correct use: <program> <file>\n", 30);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror(argv[1]);
        return 2;
    }

    char buffer[MAX_SIZE];
    read(fd, buffer, MAX_SIZE);
    close(fd);

    write(STDOUT_FILENO, buffer, strlen(buffer));

    return 0;    
}