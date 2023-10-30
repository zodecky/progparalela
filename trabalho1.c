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

#include "estruturas.h"

void escalona(Fila *processos, int numlinhas);
int numlinhasarq(FILE *fp);
void novaentrada(const char path[], Fila *processos);

int tempo = 0;
CircularLinkedList *lista_processos = NULL;

void adicionar_processo(Fila *processo)
{
    CircularLinkedList *novo = (CircularLinkedList *)malloc(sizeof(CircularLinkedList));
    novo->processo = processo;
    novo->next = NULL;

    if (lista_processos == NULL)
    {
        lista_processos = novo;
        lista_processos->next = lista_processos;
    }
    else
    {
        CircularLinkedList *ultimo = lista_processos;
        while (ultimo->next != lista_processos)
        {
            ultimo = ultimo->next;
        }
        ultimo->next = novo;
        novo->next = lista_processos;
    }
}


void setflag(int sig)
{
    printf("+10s");
    tempo += 10;
    return;
}


int main(void)
{
    signal(SIGINT, setflag);

    FILE *fp = fopen("exec.txt", "r");
    int numlinhas = numlinhasarq(fp);

    printf("Numero de linhas: %d\n", numlinhas);
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