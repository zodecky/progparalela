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

#define SEM_NAME_LEITURA "/sem_leitura"
#define SEM_NAME_ESCRITA "/sem_escrita"

int main(int argc, char const *argv[])
{
    // ENTRADA / MEMORIA COMPARTILHADA
    char message[MAX_SIZE];

    int shmid = shmget(SHM_KEY, MAX_SIZE, 0644 | IPC_CREAT);
    char *data = shmat(shmid, NULL, 0);

    // SEMAFORO
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
;

for (int i = 0; i < LOOP_SIZE; i++)
{
    sprintf(message, "mensagem %d\n", i);
    // print message
    printf("MSG: %s", message);

    strncpy(data, message, MAX_SIZE);
    sem_post(sem_leitura); // libera o semaforo de ler
    sem_wait(sem_escrita); // espera o outro programa ler a mensagem
}

    // libera o semaforo de ler, para sair do loop (porque o wait esta embaixo)
    sem_post(sem_leitura); 
    
    return 0;
}
