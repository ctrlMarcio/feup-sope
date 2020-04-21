#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define STDERR 2

int amount = 50000;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void *thrfunc(void *arg)
{
    void *count = malloc(sizeof(int));
    *(int *)count = 0;

    fprintf(stderr, "Starting thread %s\n", (char *)arg);
    while (1)
    {
        pthread_mutex_lock(&mut);
        if (amount-- <= 0)
        {
            pthread_mutex_unlock(&mut);
            return count;
        }
        pthread_mutex_unlock(&mut);
        write(STDERR, arg, 1);
        (*(int *)count)++;
    }
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

    printf("\n%d + %d = %d\n", c1, c2, c1 + c2);

    free(counter1);
    free(counter2);

    return 0;
}