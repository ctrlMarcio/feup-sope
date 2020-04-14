#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define STDERR 2
#define NUMITER 10000

void *thrfunc(void *arg)
{
    // casts the integer to a string (to use write later)
    char tmp[10];
    sprintf(tmp, "%d", *(int *) arg);

    int i;
    fprintf(stderr, "Starting thread %s\n", tmp);
    for (i = 1; i <= NUMITER; i++)
        write(STDERR, tmp, strlen(tmp));
    return NULL;
}

int main()
{
    pthread_t ta, tb;
    int a = 1;
    pthread_create(&ta, NULL, thrfunc, &a);
    int b = 2;
    pthread_create(&tb, NULL, thrfunc, &b);
    pthread_join(ta, NULL);
    pthread_join(tb, NULL);
    return 0;
}