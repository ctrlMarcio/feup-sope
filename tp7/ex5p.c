/* Producer process */

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

#include "ex5hm.h"

#define MAX_ITEM 50

SharedMemory *map_memory();
void create_semaphores(Semaphores *sem);
void close_semaphores(Semaphores *sem);

int main()
{
    // creates shared memory
    SharedMemory *shm = map_memory();

    // creates the semaphores
    Semaphores semaphores;
    create_semaphores(&semaphores);

    srand(time(0));
    for (int i = 0; i < ITERATIONS; ++i)
    {
        // produce item
        int item = rand() % (MAX_ITEM + 1); // for MAX_ITEM = 50, generates between 0 and 50

        // wait not full
        if (sem_wait(semaphores.not_full))
        {
            perror("Wait for not full");
            exit(SEM_ERROR);
        }

        // wait mutex
        if (sem_wait(semaphores.mutex))
        {
            perror("Wait for \"mutex\"");
            exit(SEM_ERROR);
        }

        // puts the item
        int place;
        sem_getvalue(semaphores.not_empty, &place);
        shm->stock[place] = item;

        // signal mutex
        if (sem_post(semaphores.mutex))
        {
            perror("Post to \"mutex\"");
            exit(SEM_ERROR);
        }

        // signal not empty
        if (sem_post(semaphores.not_empty))
        {
            perror("Post to not empty");
            exit(SEM_ERROR);
        }

        // prints confirmation
        printf("Saved %d into %d\n", item, place);
    }

    sleep(5); // room of maneuver to the consumer, in an ideal world i would send a signal or smth, but im tired
    // closes and unlinks the semaphores
    close_semaphores(&semaphores);

    // unlinks shared memory
    shm_unlink(SHM_NAME);
}

SharedMemory *map_memory()
{
    SharedMemory shm;
    shm.filled_size = 0;

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0777);
    if (shm_fd == -1)
    {
        perror("Opening shared memory");
        exit(SHM_ERROR);
    }

    if (ftruncate(shm_fd, sizeof(shm)))
    {
        perror("Truncate");
        exit(SHM_ERROR);
    }

    void *shm_ptr = mmap(0, sizeof(shm), PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED)
    {
        perror("Mapping");
        exit(SHM_ERROR);
    }

    return (SharedMemory *)shm_ptr;
}

void create_semaphores(Semaphores *semaphores)
{
    // creates not full semaphore
    sem_t *sem = sem_open(NOT_FULL, O_CREAT, 0777, MAX_SIZE);
    if (sem == SEM_FAILED)
    {
        perror("Creating not full");
        exit(SEM_ERROR);
    }

    semaphores->not_full = sem;

    // creates not empty semaphore
    sem = sem_open(NOT_EMPTY, O_CREAT, 0777, 0);

    if (sem == SEM_FAILED)
    {
        perror("Creating not empty");
        exit(SEM_ERROR);
    }

    semaphores->not_empty = sem;

    // creates "mutex" semaphore
    sem = sem_open(MUTEX, O_CREAT, 0777, 1);

    if (sem == SEM_FAILED)
    {
        perror("Creating \"mutex\"");
        exit(SEM_ERROR);
    }

    semaphores->mutex = sem;
}

void close_semaphores(Semaphores *semaphores)
{
    if (!sem_close(semaphores->not_full))
    {
        if (sem_unlink(NOT_FULL))
            perror("Unlink not full");
    }
    else
        perror("Close not full");

    if (!sem_close(semaphores->not_empty))
    {
        if (sem_unlink(NOT_EMPTY))
            perror("Unlink not empty");
    }
    else
        perror("Close not empty");

    if (!sem_close(semaphores->mutex))
    {
        if (sem_unlink(MUTEX))
            perror("Unlink mutex");
    }
    else
        perror("Close mutex");
}
