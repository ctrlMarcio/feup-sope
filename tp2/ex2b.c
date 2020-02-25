#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

const int MAX_SIZE = 10000;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        write(STDOUT_FILENO, "Correct use: <program> <src> <dest>\n", 36);
        return 1;
    }

    int fdold = open(argv[1], O_RDONLY);
    if (fdold == -1) {
        perror(argv[1]);
        return 2;
    }

    char buffer[MAX_SIZE];
    read(fdold, buffer, MAX_SIZE);
    close(fdold);

    int fdnew = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0700);
    if (fdnew == -1) {
        perror(argv[2]);
        return 3;
    }
    
    write(fdnew, buffer, strlen(buffer));
    close(fdnew);

    return 0;    
}