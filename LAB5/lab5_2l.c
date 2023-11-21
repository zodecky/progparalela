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

// signal
#include <signal.h>

#include "config.h"
#include "stringqueue.h"

#define SEM_NAME_LEITURA "/sem_leitura"
#define SEM_NAME_ESCRITA "/sem_escrita"
#define SEM_NAME_MUTEX "/sem_mutex"

void interruptHandler(int sig)
{
    printf("\nlimpando memoria e nomes...\n");
    sem_unlink(SEM_NAME_LEITURA);
    sem_unlink(SEM_NAME_ESCRITA);
    sem_unlink(SEM_NAME_MUTEX);
    exit(0);
}

int main(int argc, char const *argv[])
{
    // limpa ao ser interrompido
    signal(SIGINT, interruptHandler);

    // SEMAFORO
    sem_t *sem_leitura;
    sem_t *sem_escrita;
    sem_t *sem_mutex;

    sem_leitura = sem_open(SEM_NAME_LEITURA, O_CREAT, 0666, 0); // permite ler
    sem_escrita = sem_open(SEM_NAME_ESCRITA, O_CREAT, 0666, 8); // permite escrever
    sem_mutex = sem_open(SEM_NAME_MUTEX, O_CREAT, 0666, 1);     // mutex

    if (sem_leitura == SEM_FAILED || sem_escrita == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    // MEMORIA COMPARTILHADA
    int shmid = shmget(SHM_KEY, MAX_SIZE, 0644);
    Queue *q = (Queue *)shmat(shmid, NULL, 0);

    //        LEITURA
    printf("Leitor: esperando escritor iniciar\n");
    sem_wait(sem_leitura);

    for (int i = 0; i < LOOP_SIZE; i++)
    {
        sem_wait(sem_mutex); // lock
        printf("recebido: %s", dequeue(q).str);
        sem_post(sem_mutex); // unlock

        sem_post(sem_escrita); // permite mais uma escrita

        sem_wait(sem_leitura); // espera poder ler novamente
    }

    printf("Leitor: liberando semaforo\n");
    sem_unlink(SEM_NAME_LEITURA);
    sem_unlink(SEM_NAME_ESCRITA);
    sem_unlink(SEM_NAME_MUTEX);

    printf("Leitor: liberando memoria\n");
    shmdt(q);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
