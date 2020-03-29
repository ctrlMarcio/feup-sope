#include <stdio.h>
#include <stdlib.h>

#define SORT "/usr/bin/sort"
#define MAXLINE 100

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    FILE *sort;
    if (!(sort = popen(SORT, "w")))
    {
        perror("Open sort");
        exit(1);
    }

    FILE *fpin;
    if (!(fpin = fopen(argv[1], "r")))
    {
        perror("Open file");
        exit(2);
    }

    char line[MAXLINE];
    while (fgets(line, MAXLINE, fpin))
        fputs(line, sort);

    pclose(sort);
    fclose(fpin);

    exit(0);
}