#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct strings {
    char string[1024];
    char file[1024];
} strings;

void *get_line(void *arg);

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: %s <string> <filenames>\n", argv[0]);
        exit(1);
    }

    int fileno = argc - 2;
    pthread_t threads[fileno];
    struct strings strings[fileno];
    for (int i = 0; i < fileno; ++i) {
        struct strings str = {"", ""};
        strcpy(str.string, argv[1]);
        strcpy(str.file, argv[i + 2]);
        strings[i] = str;
        pthread_create(&threads[i], NULL, get_line, &strings[i]);
    }

    void *lines[fileno];
    for (int i = 0; i < fileno; ++i) {
        pthread_join(threads[i], &lines[i]);

        int line = * (int *)lines[i];
        if (line)
            printf("%s - line %d\n", strings[i].file, line);
        else
            printf("%s - The string is non existent\n", strings[i].file);

        free(lines[i]);
    }

    exit(0);
}

void *get_line(void *arg)
{
    struct strings *strings = (struct strings *)arg;
    char *string = strings->string;
    char *fileName = strings->file;

    FILE * file;
    if (!(file = fopen(fileName, "r"))) {
        perror(strings->file);
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

    void *res = malloc(sizeof(int));
    * (int *)res = found;
    return res;
}