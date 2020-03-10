#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

void copy(char *src, char *dst) {
    int fd_dst = open(dst, O_WRONLY | O_CREAT | O_EXCL);
    if (fd_dst < 0) { // error on opening the destiny file
        perror(dst);
        return;
    }

    int fd_src = open(src, O_RDONLY);
    if (fd_src < 0) { // error on opening the source file
        perror(src);
        return;
    }

    char c[1];
    while(read(fd_src, c, 1)) // reads to a char, writing it right away
        write(fd_dst, c, 1);
}

int main(int argc, char *argv[])
{
    if (argc != 3) { // wrong usage
        printf("Usage: %s <dir1> <dir2>\n", argv[0]);
        exit(1);
    }

    DIR *src;
    struct dirent *dentry;
    struct stat stat_entry;
    char source_file[300];
    char destiny_file[300];
    pid_t pid;

    src = opendir(argv[1]);
    if (!src) {
        printf("Usage: %s <dir1> <dir2>\n", argv[0]);
        exit(2);
    }

    if (!opendir(argv[2])) // creates the destiny directory if it doesn't exist
        mkdir(argv[2], 0744);

    while ((dentry = readdir(src)) != NULL) // for all the dirent in the source directory
    {
        pid = fork();

        if (pid == 0) { // lets the copy for its child and goes on
            // formatting of the source and destiny filenames
            sprintf(source_file, "%s/%s", argv[1], dentry->d_name);
            sprintf(destiny_file, "%s/%s", argv[2], dentry->d_name);

            // stats for the source file (each file on the while loop)
            lstat(source_file, &stat_entry);

            if (S_ISREG(stat_entry.st_mode)) // only copies regular files
                copy(source_file, destiny_file);

            exit(0);    
        }

        waitpid(0, NULL, WNOHANG); // TODO: mby wait for each child individually
    }

    exit(0);
}