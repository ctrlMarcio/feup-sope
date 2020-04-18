#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct numbers
{
    int a, b;
} numbers;

void *sum(void *args)
{
    struct numbers *numbers = (struct numbers *)args;

    int a = numbers->a;
    int b = numbers->b;

    void *res = malloc(sizeof(int));
    *(int *)res = a + b;
    return res;
}

void *subtract(void *args)
{
    struct numbers *numbers = (struct numbers *)args;

    int a = numbers->a;
    int b = numbers->b;

    void *res = malloc(sizeof(int));
    *(int *)res = a - b;
    return res;
}

void *multiply(void *args)
{
    struct numbers *numbers = (struct numbers *)args;

    int a = numbers->a;
    int b = numbers->b;

    void *res = malloc(sizeof(int));
    *(int *)res = a * b;
    return res;
}

void *divide(void *args)
{
    struct numbers *numbers = (struct numbers *)args;

    int a = numbers->a;
    int b = numbers->b;

    void *res = malloc(sizeof(int));
    *(int *)res = a / b;
    return res;
}

int main()
{
    pthread_t threads[4];
    struct numbers numbers;
    void *res[4];

    printf("The numbers pretty please (a b) ");
    scanf("%d %d", &(numbers.a), &(numbers.b));

    pthread_create(&threads[0], NULL, sum, &numbers);
    pthread_create(&threads[1], NULL, subtract, &numbers);
    pthread_create(&threads[2], NULL, multiply, &numbers);
    pthread_create(&threads[3], NULL, divide, &numbers);

    pthread_join(threads[0], &res[0]);
    printf("%d + %d = %d\n", numbers.a, numbers.b, *(int *)(res[0]));
    free(res[0]);

    pthread_join(threads[1], &res[1]);
    printf("%d - %d = %d\n", numbers.a, numbers.b, *(int *)(res[1]));
    free(res[1]);

    pthread_join(threads[2], &res[2]);
    printf("%d * %d = %d\n", numbers.a, numbers.b, *(int *)(res[2]));
    free(res[2]);

    pthread_join(threads[3], &res[3]);
    printf("%d / %d = %d\n", numbers.a, numbers.b, *(int *)(res[3]));
    free(res[3]);

    exit(0);
}