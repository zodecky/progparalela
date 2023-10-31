#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>

#include "estruturas.h"

extern int tempo;
extern CircularLinkedList *primeiro;
extern CircularLinkedList *lista_processos;

void escalona(Fila *processos, int numlinhas);
int numlinhasarq(FILE *fp);
void novaentrada(const char path[], Fila *processos);
void adicionar_roundrobin(Fila *processo);

int call(pid_t pid, int sig)
{
    if (pid != 0)
    {
        return kill(pid, sig);
    }

    return -1;
}

void escalona(Fila *processos, int numlinhas)
{

    // escalona os processos
    pid_t processo_atual = -1;
    int qtd_round_robin = 0;

    bool realtime_executando = false;
    bool processo_executando = false;

    sleep(1);

    while (true)
    {

        // algum processo precisa ser inicializado?

        for (int i = 0; i < numlinhas; i++)
        {

            if (processos[i].pid != 0 || processos[i].criado == false) // ja foi ini ou nao foi criado
            {
                continue;
            }

            // cria o processo
            pid_t pid = fork();

            if (pid == 0) // FILHO EXECUTA
            {
                if (processos[i].executando == true) // ja esta executando
                {
                    printf("\033[1;33mProcesso %s ja foi inicializado.\n\033[0m", processos[i].nome);
                    exit(0);
                }

                processos[i].executando = true;

                char nomeprogexec[30];
                nomeprogexec[0] = '.';
                nomeprogexec[1] = '/';
                strcpy(nomeprogexec + 2, processos[i].nome);
                // processo filho
                // executa o programa
                // formata a string

                execvp(nomeprogexec, NULL);
                printf("\033[1;31mErro ao executar o programa %s.\n\033[0m", processos[i].nome);
                exit(0); // so executa se o programa não existir
            }
            else
            {
                // PAI CONTROLA
                printf("\033[1;32mProcesso %s criado com pid %d\n\033[0m", processos[i].nome, pid);

                // processo pai
                // salva o pid do processo
                processos[i].pid = pid;
                call(pid, SIGSTOP); // pausa o processo

                // se o processo for round robin, adiciona ele na lista de round robin
                if (processos[i].tipo == ROUND_ROBIN)
                {
                    adicionar_roundrobin(&processos[i]);
                    qtd_round_robin++;

                    // se um novo processo round robin foi criado, reinicia a lista de round robin
                    lista_processos = primeiro; // anti bug, não serve para larga escala
                }

                // print lista rr
                CircularLinkedList *temp = lista_processos;
                printf("\033[0;35mLista de processos round robin:\n\033[0m");
                for (int i = 0; i < qtd_round_robin; i++)
                {
                    printf("\033[0;35m%s, pid = %d\n\033[0m", temp->processo->nome, temp->processo->pid);
                    temp = temp->next;
                }
            }
        }

        if (tempo == 0)
        {
            // pausa todos os processos
            for (int i = 0; i < numlinhas; i++)
            {
                call(processos[i].pid, SIGSTOP);
            }
        }

        // algum tempo de processo real time terminou?
        for (int i = 0; i < numlinhas; i++)
        {
            if (processos[i].tipo == REAL_TIME && processos[i].inicio + processos[i].duracao == tempo)
            {
                call(processos[i].pid, SIGSTOP);
                processo_atual = -1;

                realtime_executando = false;
                processo_executando = false;
            }
        }

        // algum processo real time gostaria de executar?
        for (int i = 0; i < numlinhas; i++)
        {
            if (processos[i].tipo == REAL_TIME && processos[i].inicio == tempo && processos[i].criado == true)
            {
                // se existe algum processo real time executando, emite erro
                if (realtime_executando)
                {
                    // emite um erro que o processo não pode ser executado
                    printf("\033[1;33mErro: processo %s não pode ser executado. Processo pid %d em execucao\n\033[0m", processos[i].nome, processo_atual);
                }
                else
                {
                    // se existe algum processo round robin executando, pausa ele
                    if (processo_atual != -1 && qtd_round_robin > 0)
                    {
                        call(processo_atual, SIGSTOP);
                    }
                    processo_atual = processos[i].pid;
                    call(processos[i].pid, SIGCONT);

                    realtime_executando = true;
                    processo_executando = true;
                }
            }
        }

        // caso nenhum processo real time esteja executando, executa um processo round robin, alternando entre eles a cada 1 segundo
        if (!realtime_executando && qtd_round_robin > 0) // qtd evita segfault
        {
            if (processo_atual == -1)
            {
                // se nenhum processo esta executando, executa o proximo da lista
                if (lista_processos != NULL)
                {
                    call(lista_processos->processo->pid, SIGCONT);
                    processo_atual = lista_processos->processo->pid;

                    // move o ponteiro para o proximo processo
                    lista_processos = lista_processos->next;
                }
            }
            else
            {
                // se algum processo esta executando, pausa ele e executa o proximo
                call(processo_atual, SIGSTOP);
                call(lista_processos->processo->pid, SIGCONT);

                // executa o proximo processo
                processo_atual = lista_processos->processo->pid;

                // move o ponteiro para o proximo processo
                lista_processos = lista_processos->next;

            }

            processo_executando = true;
            realtime_executando = false;
        }

        if (!processo_executando)
        {
            printf("\033[1;33mNenhum processo executando.\n\033[0m");
            processo_executando = false;
            realtime_executando = false;
        }

        sleep(1); // tempo de atualização
        printf("\033[1;34m - %ds\n\033[0m", tempo);
        tempo += 1;
        tempo %= 60;
    }
}