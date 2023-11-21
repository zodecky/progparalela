#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <semaphore.h>

#define SEM_NAME "/semaforo"

int main(int argc, char const *argv[])
{
    sem_t *sem;

    sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);

    if (sem == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }
;

    printf("Leitor: esperando\n");
    sem_wait(sem);
    printf("Leitor: leu\n");

    printf("Leitor: liberando semaforo\n");
    sem_post(sem);
    sem_unlink(SEM_NAME);

    return 0;
}
