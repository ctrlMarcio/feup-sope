#include <stdio.h>
#include <pthread.h>

void *sum(void *args) {
    int* arr = (int *)args;

    int a = arr[0];
    int b = arr[1];

    printf("%d + %d = %d\n", a, b, a + b);

    return NULL;
}

void *subtract(void *args) {
    int* arr = (int *)args;

    int a = arr[0];
    int b = arr[1];

    printf("%d - %d = %d\n", a, b, a - b);

    return NULL;
}

void *multiply(void *args) {
    int* arr = (int *)args;

    int a = arr[0];
    int b = arr[1];

    printf("%d * %d = %d\n", a, b, a * b);

    return NULL;
}

void *divide(void *args) {
    int* arr = (int *)args;

    int a = arr[0];
    int b = arr[1];

    printf("%d / %d = %d\n", a, b, a / b);

    return NULL;
}

int main() {
    int numbers[2];
    pthread_t threads[4];

    printf("The numbers pretty please (a b) ");
    scanf("%d %d", &numbers[0], &numbers[1]);

    pthread_create(&threads[0], NULL, sum, numbers);
    pthread_create(&threads[1], NULL, subtract, numbers);
    pthread_create(&threads[2], NULL, multiply, numbers);
    pthread_create(&threads[3], NULL, divide, numbers);

    pthread_exit(0);
}