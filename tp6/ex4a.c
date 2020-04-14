#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define N 5

void *thread(void *arg)
{
    sleep(1);

    printf("%d pid: %lu\n", *(int *)arg, (unsigned long)pthread_self());

    // "return an equal number"
    void *res = malloc(sizeof(int));
    *(int *)res = *(int *)arg;
    return res;
}

int main()
{
    pthread_t threads[N];
    int args[N];
    void *results[N];

    for (int i = 0; i < N; ++i)
    {
        args[i] = i;
        pthread_create(&threads[i], NULL, thread, &args[i]);
    }

    for (int i = 0; i < N; ++i)
    {
        pthread_join(threads[i], &results[i]);
    }

    // using results for smth

    for (int i = 0; i < N; ++i)
    {
        free(results[i]);
    }

    pthread_exit(0);
}