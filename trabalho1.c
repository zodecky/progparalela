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
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <ctype.h>

#define MAX 100 // tamanho maximo de uma linha

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

int numlinhasarq(FILE *fp)
{
    int numlinhas = 0;
    char c;
    bool temtexto = 0;

    for (c = (char)getc(fp); c != EOF; c = (char)getc(fp))
    {
        if (c == '\n')
        {
            if (temtexto)
            {
                numlinhas++;
                temtexto = 0;
            }
        }
        else
        {
            temtexto = 1;
        }
    }

    // se a ultima linha não termina com \n
    if (temtexto)
    {
        numlinhas++;
    }

    rewind(fp);

    return numlinhas;
}

Fila *novaentrada(const char path[])
{
    FILE *fp = fopen(path, "r");
    int i = 0;

    char nomeprocesso[MAX];

    int argi = 0;
    int argd = 0;

    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    // descobre o numero de linhas do arquivo
    int numlinhas = numlinhasarq(fp);

    // aloca memoria para o vetor de processos
    Fila *processos = (Fila *)malloc(numlinhas * sizeof(Fila));
    if (processos == NULL)
    {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    int conta_linha = 1;
    char linha[MAX];

    while (fgets(linha, MAX, fp) != NULL)
    {
        // formatos aceitos:
        // Run prog1 I=10 D=10
        // Run prog1

        // verifica se a linha é valida
        if (strncmp(linha, "Run", 3) != 0)
        {
            printf("Erro na linha %d: comando invalido.\n", conta_linha);
            exit(1);
        }

        // verifica se a linha tem 2 ou 4 argumentos
        int numargs = 0;
        for (int k = 0; k < MAX; k++)
        {
            if (linha[k] == ' ')
            {
                numargs++;
            }
        }

        if (numargs != 1 && numargs != 3)
        {
            printf("Erro na linha %d: numero de argumentos invalido.\n", conta_linha);
            exit(1);
        }

        // verifica se o nome do programa é valido
        char *nomeprog = strtok(linha, " ");
        nomeprog = strtok(NULL, " ");

        strcpy(nomeprocesso, nomeprog);

        if (nomeprog == NULL)
        {
            printf("Erro na linha %d: nome do programa invalido.\n", conta_linha);
            exit(1);
        }

        // verifica se o nome do programa tem no maximo 10 caracteres
        if (strlen(nomeprog) > 10)
        {
            printf("Erro na linha %d: nome do programa invalido.\n", conta_linha);
            exit(1);
        }

        // verifica se o nome do programa é valido
        if (strncmp(nomeprog, "Run", 3) == 0)
        {
            printf("Erro na linha %d: nome do programa nao pode ser Run.\n", conta_linha);
            exit(1);
        }
        // verifica se inclui os argumentos I e D
        if (numargs == 3)
        {
            char *inicio = strtok(NULL, " ");
            char *duracao = strtok(NULL, " ");

            if (inicio == NULL || duracao == NULL)
            {
                printf("Erro na linha %d: argumentos invalidos.\n", conta_linha);
                exit(1);
            }

            // verifica se o argumento I é valido
            if (strncmp(inicio, "I=", 2) != 0)
            {
                printf("Erro na linha %d: argumento I nao existe.\n", conta_linha);
                exit(1);
            }

            // verifica se o argumento D é valido
            if (strncmp(duracao, "D=", 2) != 0)
            {
                printf("Erro na linha %d: argumento D nao existe.\n", conta_linha);
                exit(1);
            }

            // verifica se o argumento I é um numero
            for (int i = 2; i < strlen(inicio); i++)
            {
                if (duracao[i] == '\n')
                {
                    continue; // ignorar newline (anti-bug)
                }
                if (!isdigit(inicio[i]))
                {
                    printf("Erro na linha %d: argumento I precisa ser um numero.\n", conta_linha);
                    exit(1);
                }
            }
            argi = atoi(inicio + 2);

            // verifica se o argumento D é um numero
            for (int i = 2; i < strlen(duracao); i++)
            {
                if (duracao[i] == '\n')
                {
                    continue; // ignorar newline (anti-bug)
                }

                if (!isdigit(duracao[i]))
                {
                    printf("Erro na linha %d: argumento D precisa ser um numero.\n", conta_linha);
                    exit(1);
                }
            }

            argd = atoi(duracao + 2);

            // verifica se o argumento I é valido
            int inicio_int = atoi(inicio + 2);
            if (inicio_int <= 0 || inicio_int >= 60)
            {
                printf("Erro na linha %d: argumento I nao pode ser negativo nem maior que 60.\n", conta_linha);
                exit(1);
            }

            // verifica se o argumento D é valido
            int duracao_int = atoi(duracao + 2);
            if (duracao_int <= 0 || duracao_int >= 60)
            {
                printf("Erro na linha %d: argumento D nao pode ser negativo nem maior que 60.\n", conta_linha);
                exit(1);
            }

            // verifica se a soma de I e D é maior que 60

            if (inicio_int + duracao_int > 60)
            {
                printf("Erro na linha %d: i + d não pode ser maior que 60.\n", conta_linha);
                exit(1);
            }
        }
        conta_linha++;

        // se passou nos testes acima, a linha é valida. Adiciona o processo na fila

        // remove \n do final do nome do processo, se existir
        if (nomeprocesso[strlen(nomeprocesso) - 1] == '\n')
        {
            nomeprocesso[strlen(nomeprocesso) - 1] = '\0';
        }

        strcpy(processos[i].nome, nomeprocesso);
        processos[i].pid = 0; // ainda não foi criado
        processos[i].tipo = numargs == 3 ? REAL_TIME : ROUND_ROBIN;

        processos[i].inicio = numargs == 3 ? argi : -1;
        processos[i].duracao = numargs == 3 ? argd : -1;

        i++; // proximo processo
    }

    fclose(fp);

    return processos;
}

void escalona(Fila *processos, int numlinhas)
{
    for (int i = 0; i < numlinhas; i++)
    {
        // cria o processo
        pid_t pid = fork();

        if (pid == 0)
        {
            // processo filho
            // executa o programa
            execvp(processos[i].nome, NULL);
            printf("Erro ao executar o programa %s.\n", processos[i].nome);
            exit(0); // so executa se o programa não existir
        }
        else
        {
            // processo pai
            // salva o pid do processo
            processos[i].pid = pid;
            kill(pid, SIGSTOP); // pausa o processo
        }
    }

    // escalona os processos
    int tempo = 0;

    while (true)
    {
        }
}

int main(void)
{
    FILE *fp = fopen("exec.txt", "r");
    int numlinhas = numlinhasarq(fp);

    printf("Numero de linhas: %d\n", numlinhas);
    fclose(fp);

    Fila *processos = novaentrada("exec.txt");

    for (int i = 0; i < numlinhas; i++)
    {
        printf("Nome: %s\n", processos[i].nome);
        printf("PID: %d\n", processos[i].pid);
        printf("Tipo: %s\n", processos[i].tipo == REAL_TIME ? "tempo_real" : "round_robin");
        printf("Inicio: %d\n", processos[i].inicio);
        printf("Duracao: %d\n", processos[i].duracao);
        printf("\n");
    }

    escalona(processos, numlinhas);
    return 0;
}