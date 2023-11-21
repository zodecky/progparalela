#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

// mem
#include <sys/ipc.h>
#include <sys/shm.h>

// sem
#include <semaphore.h>

#include "config.h"

#define SEM_NAME "/semaforo"

int main(int argc, char const *argv[])
{

    // SEMAFORO
    sem_t *sem;
    sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);

    if (sem == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    // MEMORIA COMPARTILHADA
    int shmid = shmget(SHM_KEY, MAX_SIZE, 0644);
    char *data = shmat(shmid, NULL, 0);


    //        LEITURA 
    printf("Leitor: esperando msg do dia\n");
    sem_wait(sem);
    printf("Leitor: leu\n");

    printf("Mensagem do dia: %s", data);

    printf("Leitor: liberando semaforo\n");
    sem_post(sem);
    sem_unlink(SEM_NAME);

    printf("Leitor: liberando memoria\n");
    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
