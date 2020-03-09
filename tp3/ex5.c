#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main()
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid == -1) // error
        perror("First fork");
    else if (pid == 0) // child
        write(STDOUT_FILENO, "Hello ", 6); // Runs first because of a second fork afterwards in the pparent
    else
    { // parent
        wait(&status);

        // in case something goes wrong
        if (status == -1) {
            perror("Waiting for first fork error");
            return 1;
        }

        pid = fork();

        if (pid == -1) // error
            perror("Second fork");
        else if (pid == 0) // child
            write(STDOUT_FILENO, "friends!\n", 9); // parent runs faster
        else // parent
            write(STDOUT_FILENO, "my ", 3);
    }
}