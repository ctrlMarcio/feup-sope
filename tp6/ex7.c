#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

struct copy_struct
{
    char src[100], dst[100];
} copy_struct;

void *copy(void *strct)
{
    struct copy_struct *cpy = (struct copy_struct *)strct;

    FILE *source, *destiny;
    char *src = cpy->src;
    char *dst = cpy->dst;

    printf("%s\n", src);

    source = fopen(src, "r");
    if (source == NULL)
    {
        perror(src);
        exit(2);
    }

    destiny = fopen(dst, "w");
    if (destiny == NULL)
    {
        fclose(source);
        perror(dst);
        exit(2);
    }

    char ch;
    while ((ch = fgetc(source)) != EOF)
        fputc(ch, destiny);

    fclose(source);
    fclose(destiny);
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <srcdir> <destdir>\n", argv[0]);
        exit(1);
    }

    DIR *dir;
    struct dirent *dirent;
    struct stat stat_entry;

    if ((dir = opendir(argv[1])) == NULL)
    {
        perror(argv[1]);
        exit(2);
    }

    struct copy_struct arr[100];
    int i = 0;

    while ((dirent = readdir(dir)) != NULL)
    {
        lstat(dirent->d_name, &stat_entry);

        if (S_ISREG(stat_entry.st_mode))
        {
            pthread_t id;

            char dst[1024] = "";
            sprintf(dst, "%s/%s", argv[2], dirent->d_name);

            char src[1024] = "";
            sprintf(src, "%s/%s", argv[1], dirent->d_name);

            struct copy_struct cpy = {"", ""};
            strcpy(cpy.src, src);
            strcpy(cpy.dst, dst);
            arr[i] = cpy;

            pthread_create(&id, NULL, copy, &arr[i++]);
        }
    }

    pthread_exit(0);
}