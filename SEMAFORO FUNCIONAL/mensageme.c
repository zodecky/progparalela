#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <semaphore.h>

#define SEM_NAME "semaforo"

int main(int argc, char const *argv[])
{
    // ENTRADA / MEMORIA COMPARTILHADA
    char message[MAX_SIZE];

    printf("Digite a mensagem do dia: ");
    fgets(message, MAX_SIZE, stdin);

    int shmid = shmget(SHM_KEY, MAX_SIZE, 0644 | IPC_CREAT);
    char *data = shmat(shmid, NULL, 0);

    // SEMAFORO
    sem_t *sem;

    sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);

    if (sem == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }
;

    printf("Escritor: escrevendo\n");
    strncpy(data, message, MAX_SIZE);
    sleep(2);

    printf("Escritor: escreveu\n");
    printf("Escritor: liberando semaforo\n");
    sem_post(sem);

    shmdt(data);

    return 0;
}
