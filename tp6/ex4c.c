#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define N 5

void *thread(void *arg)
{
    sleep(1);

    printf("%d pid: %ld\n", *(int *)arg, (long)pthread_self());

    // return an equal number
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

    // could be all inside the same for cicle, but this way the presentation is prettier
    printf("res: ");
    for (int i = 0; i < N; ++i)
    {
        printf("%d ", *(int *)results[i]);
        free(results[i]);
    }
    printf("\n");

    exit(0);
}