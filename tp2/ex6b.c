#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

void printInfo(struct dirent *direntp);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf( stderr, "Usage: %s dir_name\n", argv[0]);
        exit(1);
    }

    DIR *dirp;
    if ((dirp = opendir( argv[1])) == NULL) {
        perror(argv[1]);
        exit(2);
    }

    chdir(argv[1]);

    struct dirent *direntp;
    while ((direntp = readdir( dirp)) != NULL) {
        printInfo(direntp);
    }

    closedir(dirp);
    exit(0);
} 

void printInfo(struct dirent *direntp) {
    struct stat stat_buf;
    char *type;

    if (stat(direntp->d_name, &stat_buf) < 0)
        perror(direntp->d_name);

    if (S_ISREG(stat_buf.st_mode))
        type = "regular";
    else if (S_ISDIR(stat_buf.st_mode))
        type = "directory";
    else
        type = "other";
    
    int inode = stat_buf.st_ino;

    printf("%-25s - %s\t%d", direntp->d_name, type, inode);
    if (S_ISREG(stat_buf.st_mode))
        printf("\t%ld", stat_buf.st_size);
    printf("\n");
}