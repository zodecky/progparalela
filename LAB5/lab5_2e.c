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

#include <signal.h>

#include <semaphore.h>

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

/**
 * Cria uma mensagem com o numero passado
 */
mensagem makemsg(int num)
{
    mensagem msg;
    sprintf(msg.str, "mensagem %d\n", num);
    return msg;
}

int main(int argc, char const *argv[])
{
    // limpa ao ser interrompido
    signal(SIGINT, interruptHandler);

    // ENTRADA / MEMORIA COMPARTILHADA
    char message[MAX_SIZE];

    int shmid = shmget(SHM_KEY, MAX_SIZE, 0644 | IPC_CREAT);

    Queue *q = (Queue *)shmat(shmid, NULL, 0);
    if (!inicializada(q))
    {
        q->ini = q->fim = 0;
        q->inicializada = true;
    }

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

    mensagem msg = {"mensagem 0\0\0\0\0\0"}; // a partir do 9 é o para substituir

    for (int i = 0; i < LOOP_SIZE; i++)
    {
        msg = makemsg(i);

        sem_wait(sem_escrita); // checa se tem espaço para escrever

        sem_wait(sem_mutex); // checa se pode acessar a fila (crítico)
        enqueue(q, msg);     // escreve
        sem_post(sem_mutex); // libera a area critica da fila (crítico)

        printf("escrito: %s", msg.str);
        sem_post(sem_leitura); // libera o semaforo de ler +1
    }

    // libera o semaforo de ler, para sair do loop (porque o wait esta embaixo)
    sem_post(sem_leitura);

    return 0;
}
