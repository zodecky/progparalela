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

#define SEM_NAME_LEITURA "/sem_leitura"
#define SEM_NAME_ESCRITA "/sem_escrita"

int main(int argc, char const *argv[])
{

    // SEMAFORO
    sem_t *sem_leitura;
    sem_t *sem_escrita;
    sem_leitura = sem_open(SEM_NAME_LEITURA, O_CREAT, 0666, 0); // permite ler
    sem_escrita = sem_open(SEM_NAME_ESCRITA, O_CREAT, 0666, 0); // permite escrever

    if (sem_leitura == SEM_FAILED || sem_escrita == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    // MEMORIA COMPARTILHADA
    int shmid = shmget(SHM_KEY, MAX_SIZE, 0644);
    char *data = shmat(shmid, NULL, 0);


    //        LEITURA 

    printf("Leitor: esperando escritor iniciar\n");
    sem_wait(sem_leitura);

    for (int i = 0; i < LOOP_SIZE; i++)
    {
        printf("recebido: %s", data);
        sem_post(sem_escrita);
        sem_wait(sem_leitura);
    }


    printf("Leitor: liberando semaforo\n");
    sem_unlink(SEM_NAME_LEITURA);
    sem_unlink(SEM_NAME_ESCRITA);

    printf("Leitor: liberando memoria\n");
    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
