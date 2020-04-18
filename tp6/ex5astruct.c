#include <stdio.h>
#include <pthread.h>

struct numbers {
    int a, b;
} numbers;

void *sum(void *args) {
    struct numbers *numbers = (struct numbers *)args;

    int a = numbers->a;
    int b = numbers->b;

    printf("%d + %d = %d\n", a, b, a + b);

    return NULL;
}

void *subtract(void *args) {
    struct numbers *numbers = (struct numbers *)args;

    int a = numbers->a;
    int b = numbers->b;

    printf("%d - %d = %d\n", a, b, a - b);

    return NULL;
}

void *multiply(void *args) {
    struct numbers *numbers = (struct numbers *)args;

    int a = numbers->a;
    int b = numbers->b;

    printf("%d * %d = %d\n", a, b, a * b);

    return NULL;
}

void *divide(void *args) {
    struct numbers *numbers = (struct numbers *)args;

    int a = numbers->a;
    int b = numbers->b;

    printf("%d / %d = %d\n", a, b, a / b);

    return NULL;
}

int main() {
    pthread_t threads[4];
    struct numbers numbers;

    printf("The numbers pretty please (a b) ");
    scanf("%d %d", &(numbers.a), &(numbers.b));

    pthread_create(&threads[0], NULL, sum, &numbers);
    pthread_create(&threads[1], NULL, subtract, &numbers);
    pthread_create(&threads[2], NULL, multiply, &numbers);
    pthread_create(&threads[3], NULL, divide, &numbers);

    pthread_exit(0);
}