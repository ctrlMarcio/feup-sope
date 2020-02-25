#include <stdio.h>
#include <string.h>

const int MAX_SIZE = 10000;

int main(int argc, char *argv[]) {
    if (argc != 3)
        return 1;

    FILE *old = fopen(argv[1], "r");
    char buffer[MAX_SIZE];
    fread(buffer, MAX_SIZE, 1, old);
    fclose(old);

    FILE *new = fopen(argv[2], "w");
    fwrite(buffer, strlen(buffer), 1, new);
    fclose(new);

    return 0;
}
