#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

const int BUFF_SIZE = 50;

/**
 * Splits the string str by a delimitor, into a array dest, returning the number of elements of the array.
 */
int split(char *str, char *delimitor, char *dest[])
{
    char *word;
    int i = 0;

    word = strtok(str, delimitor);
    dest[i++] = word;

    while ((word = strtok(NULL, delimitor)))
        dest[i++] = word;

    return i;
}

/**
 * Replaces a string ending with \n with \0
 */
void endString(char *str)
{
    for (int i = 0; i < BUFF_SIZE; ++i)
    {
        if (str[i] == '\n')
        {
            str[i] = '\0';
            break;
        }
    }
}

/**
 * Updates the arguments in the command string according to the -o flag, filling an array with the args in it
 * 
 * @param tmp           the array to fill
 * @param command       the command as string
 * @param stdout_backup pointer to an integer that serves as backup of the STDOUT_FILENO file descriptor
 */
void updateArguments(char **tmp, char *command, int *stdout_backup)
{
    int arguments_size = split(command, " ", tmp); // gets the string into an array

    if (arguments_size >= 2 && !strcmp(tmp[arguments_size - 2], "-o")) // verifies if ends in -o <smth>
    {
        int fd = open(tmp[arguments_size - 1], O_CREAT | O_WRONLY | O_APPEND);

        *stdout_backup = dup(STDOUT_FILENO);
        dup2(fd, STDOUT_FILENO);

        arguments_size -= 2;
    }

    tmp[arguments_size] = NULL; // puts the last element of the array as null to declare the end of the string
}

int main()
{
    char command[BUFF_SIZE];
    char *tmp[20];
    int stdout_backup;

    while (1) // the loop actually ends ahead
    {
        stdout_backup = STDOUT_FILENO;
        write(STDOUT_FILENO, "minish> ", 8);

        read(STDIN_FILENO, command, BUFF_SIZE);
        endString(command);

        if (!strcmp(command, "quit")) // quits when the user enters "quit"
            break;

        updateArguments(tmp, command, &stdout_backup);

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Error creating child process, aborting");
            exit(1);
        }
        else if (pid == 0)
        { // child
            execvp(tmp[0], tmp);
            perror("Invalid command");
            dup2(stdout_backup, STDOUT_FILENO); // resets STDOUT_FILENO
            continue;
        }

        // parent continues here
        wait(NULL);
        dup2(stdout_backup, STDOUT_FILENO); // resets STDOUT_FILENO
    }
}