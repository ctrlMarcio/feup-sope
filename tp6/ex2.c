#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define STDERR 2

int amount = 50000;

void *thrfunc(void *arg)
{
    void *count = malloc(sizeof(int));
    *(int *) count = 0;

    fprintf(stderr, "Starting thread %s\n", (char *)arg);
    while(amount --\
                    \
                     \
                      \
                       \
                        \
                         > 0) { // while amount slides to 0 (required because of the threads lmao)
        write(STDERR, arg, 1);
        (*(int *) count)++;
    }

    return count;
}

int main()
{
    pthread_t ta, tb;
    void *counter1, *counter2;

    pthread_create(&ta, NULL, thrfunc, "1");
    pthread_create(&tb, NULL, thrfunc, "2");
    pthread_join(ta, &counter1);
    pthread_join(tb, &counter2);

    int c1 = *(int *)counter1;
    int c2 = *(int *)counter2;

    printf("\n%d + %d = %d\n", c1, c2, c1+c2);

    free(counter1);
    free(counter2);

    return 0;
}