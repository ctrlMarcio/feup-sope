#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

const int MAX_LENGTH = 50; // string's max length, withou NUL at the end

void getString(char *string, int size);

int main()
{
    write(STDOUT_FILENO, "Write here: ", 12);
    char string[MAX_LENGTH + 1];
    getString(string, MAX_LENGTH + 1);

    write(STDOUT_FILENO, "\nYour string: ", 14);
    write(STDOUT_FILENO, string, strlen(string));
    write(STDOUT_FILENO, "\n", 1);

    return 0;
}

void getString(char *string, int size)
{
    struct termios oldterm, newterm;
    char echo = '*';

    tcgetattr(STDIN_FILENO, &oldterm);
    newterm = oldterm;
    newterm.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &newterm);

    char ch;
    int i = 0;
    while (i < size - 1 && read(STDIN_FILENO, &ch, 1) && ch != '\n')
    { // size - 1 to fit \0
        char backsp = 127;
        if (i > 0 && ch == backsp)
        {
            string[i--] = ' ';
            write(STDOUT_FILENO, "\b \b", 3);
        }
        else if (ch != backsp)
        {
            string[i++] = ch;
            write(STDOUT_FILENO, &echo, 1);
        }
    }
    string[i] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);
}
