/* shared memory */

#include <semaphore.h>

/* errors */
#define SHM_ERROR 1
#define SEM_ERROR 2

#define ITERATIONS 20
#define MAX_SIZE 10

/* names */
const char *SHM_NAME = "/stock";
const char *NOT_FULL = "/not_full";
const char *NOT_EMPTY = "/not_empty";
const char *MUTEX = "/mutex";

typedef struct {
    int stock[MAX_SIZE];
    int filled_size;
} SharedMemory;

/* semaphores */

typedef struct {
    sem_t *not_full;
    sem_t *not_empty;
    sem_t *mutex;
} Semaphores;