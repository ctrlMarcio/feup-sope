#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int get_line(char *string, char *file_name);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <string> <filename>\n", argv[0]);
        exit(1);
    }

    int line = get_line(argv[1], argv[2]);
    if (line)
        printf("Line: %d\n", line);
    else
        printf("The string is non existent\n");

    exit(0);
}

int get_line(char *string, char *fileName)
{
    FILE * file;
    if (!(file = fopen(fileName, "r"))) {
        perror(fileName);
        exit(2);
    }

    char * line;
    int read, lineNumber = 0;
    size_t size;
    int found = 0;
    while ((read = getline(&line, &size, file)) != -1) {
        lineNumber++;

        if (strstr(line, string)) {
            found = lineNumber;
            break;
        }
    }

    fclose(file);

    return found;
}