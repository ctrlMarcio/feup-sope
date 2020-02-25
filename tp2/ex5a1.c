#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {
    int fd;
    char *text1="AAAAA";
    char *text2="BBBBB";  
    fd = open("f1.txt",O_CREAT|O_WRONLY|O_SYNC|O_APPEND,0600);

    if (fd < 0) {
        perror("f1.txt");
        return 1;
    }

    getchar();
    if (write(fd, text1, 5) < 0) {
        perror("Error writing text1");
        return 2;
    }

    getchar();
    if (write(fd, text2, 5) < 0) {
        perror("Error writing text2");
        return 3;
    }    
    close(fd);
    return 0;
} 