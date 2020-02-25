#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

const int MAX_SIZE = 10000;

int redirectOutput(char *argv[]);

int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 3) {
        write(STDOUT_FILENO, "Correct use: <program> <file> <new_file (opt)>\n", 47);
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

    int fdnew;
    if (argc == 3) {
        fdnew = redirectOutput(argv);
        if (fdnew == -1)
            return 3;
    }

    write(STDOUT_FILENO, buffer, strlen(buffer));

    if (argc == 3)
        close(fdnew);

    return 0;    
}

int redirectOutput(char *argv[]) {
    int fdnew = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0700);

    if (fdnew == -1) {
        perror(argv[2]);
        return -1;
    }

    dup2(fdnew, STDOUT_FILENO);
    return fdnew;
}
