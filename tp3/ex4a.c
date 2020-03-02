#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t pid = fork();
    int status;

    if (pid == -1)
    { // error
        perror("Fork");
    }
    else if (pid == 0)
    { // child
        write(STDOUT_FILENO, "Hello ", 6);
    }
    else
    { // parent
        wait(&status);
        if (status != -1)
        {
            write(STDOUT_FILENO, "world!\n", 7);
        }
        else
        {
            perror("Child");
            return 1;
        }
    }

    return 0;
}