// Interpretador de comandos:
// Run prog1 I=10, D=10
// Run prog2 I=30, D=10
// Run prog3 I=35, D=5
// Run prog4
// Run prog5

/*
I é o tempo de inicio (% 60).
D é a duração do processo.

verificar se i + d <= 60

O interpretador é um processo diferente do escalonador
O escalonador deve verificar se é possível executar o processo,
caso não seja, ele deve avisar que o processo não pode ser executado

O escalonador usa tanto ROUND ROBIN quanto REAL TIME, sendo que
REAL TIME tem prioridade sobre ROUND ROBIN.

se for definido um I e D, o processo é REAL TIME

interpretador lê de um arquivo de texto exec.txt ao inicio do programa

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/shm.h>
#include <pthread.h>

#include "estruturas.h"

void escalona(Fila *processos, int numlinhas);
int numlinhasarq(FILE *fp);
void novaentrada(const char path[], Fila *processos);

int tempo = 0;
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
CircularLinkedList *lista_processos = NULL;
CircularLinkedList *primeiro = NULL;

void adicionar_roundrobin(Fila *processo)
{
    pthread_mutex_lock(&mutex);

    CircularLinkedList *novo = (CircularLinkedList *)malloc(sizeof(CircularLinkedList));
    novo->processo = processo;
    novo->next = NULL;
    novo->prev = NULL;

    if (lista_processos == NULL)
    {
        lista_processos = novo;
        primeiro = novo;
        lista_processos->next = lista_processos;
        lista_processos->prev = lista_processos;
    }
    else
    {
        lista_processos->prev->next = novo;
        novo->prev = lista_processos->prev;
        novo->next = lista_processos;
        lista_processos->prev = novo;
    }

    pthread_mutex_unlock(&mutex);
}


void setflag(int sig)
{
    printf("\033[1;33m+10s\033[0m");
    tempo += 10;
    return;
}


int main(void)
{
    signal(SIGINT, setflag);

    FILE *fp = fopen("exec.txt", "r");
    int numlinhas = numlinhasarq(fp);

    fclose(fp);

    int shmid = shmget(IPC_PRIVATE, sizeof(Fila) * numlinhas, IPC_CREAT | 0666);
    Fila *processos = (Fila *)shmat(shmid, NULL, 0);

    // cria 2 processos
    int pid = fork();

    if (pid == 0)
    {
        novaentrada("exec.txt", processos);
        exit(0);
    }
    else
    {
        escalona(processos, numlinhas);
    }


    // for (int i = 0; i < numlinhas; i++)
    // {
    //     printf("Nome: %s\n", processos[i].nome);
    //     printf("PID: %d\n", processos[i].pid);
    //     printf("Tipo: %s\n", processos[i].tipo == REAL_TIME ? "tempo_real" : "round_robin");
    //     printf("Inicio: %d\n", processos[i].inicio);
    //     printf("Duracao: %d\n", processos[i].duracao);
    //     printf("\n");
    // }

    return 0;
}