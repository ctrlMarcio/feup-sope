#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t pid = fork();

    if (pid == -1) // error
        perror("Fork");
    else if (pid == 0) // child
        write(STDOUT_FILENO, "world!\n", 7);
    else // parent
        write(STDOUT_FILENO, "Hello ", 6);

    return 0;
}