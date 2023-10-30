#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>

#include "estruturas.h"

extern int tempo;
extern CircularLinkedList *lista_processos;

void escalona(Fila *processos, int numlinhas);
int numlinhasarq(FILE *fp);
Fila *novaentrada(const char path[]);
void adicionar_processo(Fila *processo);

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
    for (int i = 0; i < numlinhas; i++)
    {
        // cria o processo
        pid_t pid = fork();

        if (pid == 0)
        {
            char nomeprogexec[30];
            nomeprogexec[0] = '.';
            nomeprogexec[1] = '/';
            strcpy(nomeprogexec + 2, processos[i].nome);
            // processo filho
            // executa o programa
            // formata a string

            execvp(nomeprogexec, NULL);
            printf("Erro ao executar o programa %s.\n", processos[i].nome);
            exit(0); // so executa se o programa não existir
        }
        else
        {
            // processo pai
            // salva o pid do processo
            processos[i].pid = pid;
            call(pid, SIGSTOP); // pausa o processo
        }
    }

    // escalona os processos
    pid_t processo_atual = -1;
    int qtd_round_robin = 0;

    // cria a lista de processos round robin
    for (int i = 0; i < numlinhas; i++)
    {
        if (processos[i].tipo == ROUND_ROBIN)
        {
            adicionar_processo(&processos[i]);
        }
    }
    
    for (int i = 0; i < numlinhas; i++)
    {
        if (processos[i].tipo == ROUND_ROBIN)
        {
            qtd_round_robin++;
        }
    }

    bool realtime_executando = false;
    bool processo_executando = false;

    while (true)
    {

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
            if (processos[i].tipo == REAL_TIME && processos[i].inicio == tempo)
            {
                // se existe algum processo real time executando, emite erro
                if (realtime_executando)
                {
                    // emite um erro que o processo não pode ser executado
                    printf("Erro: processo %s não pode ser executado. Processo pid %d em execucao\n", processos[i].nome, processo_atual);
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
                    processo_atual = lista_processos->processo->pid;
                    call(lista_processos->processo->pid, SIGCONT);

                    // move o ponteiro para o proximo processo
                    lista_processos = lista_processos->next;
                }
            }
            else
            {
                // se algum processo esta executando, pausa ele e executa o proximo
                call(processo_atual, SIGSTOP);
                processo_atual = lista_processos->processo->pid;
                call(lista_processos->processo->pid, SIGCONT);

                // move o ponteiro para o proximo processo
                lista_processos = lista_processos->next;
            }

            processo_executando = true;
            realtime_executando = false;
        }
        
        if (!processo_executando)
        {
            printf("Nenhum processo executando.\n");
            processo_executando = false;
            realtime_executando = false;
        }


        sleep(1); // tempo de atualização
        printf(" - %ds\n", tempo);
        tempo += 1;
        tempo %= 60;
    }

}