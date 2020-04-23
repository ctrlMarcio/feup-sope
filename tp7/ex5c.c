/* Consumer process */

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "ex5hm.h"

SharedMemory *open_shared_memory();
void open_semaphores(Semaphores *semaphores);
void close_semaphores(Semaphores *semaphores);

int main()
{
    // opens shared memory
    SharedMemory *shm = open_shared_memory();

    // opens the semaphores
    Semaphores semaphores;
    open_semaphores(&semaphores);

    int item;

    for (int i = 0; i < ITERATIONS; ++i)
    {
        // wait not empty
        if (sem_wait(semaphores.not_empty))
        {
            perror("Wait for not empty");
            exit(SEM_ERROR);
        }

        // wait mutex
        if (sem_wait(semaphores.mutex))
        {
            perror("Wait for \"mutex\"");
            exit(SEM_ERROR);
        }

        // read item
        // consuming always from the top, the shared memory is working like a stack
        int place;
        sem_getvalue(semaphores.not_empty, &place);
        item = shm->stock[place];

        // signal mutex
        if (sem_post(semaphores.mutex))
        {
            perror("Post to \"mutex\"");
            exit(SEM_ERROR);
        }

        // signal not full
        if (sem_post(semaphores.not_full))
        {
            perror("Post to not full");
            exit(SEM_ERROR);
        }

        // prints confirmation
        printf("Read %d from %d\n", item, place);
    }

    // closes the semaphores
    close_semaphores(&semaphores);
}

SharedMemory *open_shared_memory()
{
    SharedMemory shm;
    shm.filled_size = 0;

    int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0777);
    if (shm_fd == -1)
    {
        perror("Opening shared memory");
        exit(SHM_ERROR);
    }

    void *shm_ptr = mmap(0, sizeof(shm), PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED)
    {
        perror("Mapping");
        exit(SHM_ERROR);
    }

    return (SharedMemory *)shm_ptr;
}

void open_semaphores(Semaphores *semaphores)
{
    // opens not full semaphore
    sem_t *sem = sem_open(NOT_FULL, 0);
    if (sem == SEM_FAILED)
    {
        perror("Opening not full");
        exit(SEM_ERROR);
    }

    semaphores->not_full = sem;

    // creates not empty semaphore
    sem = sem_open(NOT_EMPTY, 0);

    if (sem == SEM_FAILED)
    {
        perror("Opening not empty");
        exit(SEM_ERROR);
    }

    semaphores->not_empty = sem;

    // creates "mutex" semaphore
    sem = sem_open(MUTEX, 0);

    if (sem == SEM_FAILED)
    {
        perror("Opening \"mutex\"");
        exit(SEM_ERROR);
    }

    semaphores->mutex = sem;
}

void close_semaphores(Semaphores *semaphores)
{
    if (sem_close(semaphores->not_full))
        perror("Close not full");

    if (sem_close(semaphores->not_empty))
        perror("Close not empty");

    if (sem_close(semaphores->mutex))
        perror("Close mutex");
}
