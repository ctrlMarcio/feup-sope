#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int AMOUNT = 6;
const int SLEEP_TIME = 5;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <message>\n", argv[0]);
        exit(1);
    }

    printf("%s\n", argv[1]);
    for (int i = 0; i < AMOUNT - 1; ++i)
    {
        sleep(SLEEP_TIME);
        printf("%s\n", argv[1]);
    }

    exit(0);
}