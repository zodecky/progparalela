#ifndef _ESTRUTURAS_H_
#define _ESTRUTURAS_H_

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef enum tipo_processo
{
    REAL_TIME,
    ROUND_ROBIN
} TIPO;

// estrutura para a fila de processos
typedef struct fila_t
{
    pid_t pid;
    TIPO tipo;
    char nome[30];
    int inicio;
    int duracao;
} Fila;

typedef struct circular_linked_list_t
{
    Fila *processo;
    struct circular_linked_list_t *next;
} CircularLinkedList;


#endif