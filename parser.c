#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "estruturas.h"

#define MAX 100 // tamanho maximo de uma linha

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



void novaentrada(const char path[], Fila *processos)
{
    FILE *fp = fopen(path, "r");
    int i = 0;

    char nomeprocesso[MAX];

    int argi = 0;
    int argd = 0;

    if (fp == NULL)
    {
        printf("\033[1;31mErro ao abrir o arquivo.\n\033[0m");
        exit(1);
    }

    // descobre o numero de linhas do arquivo
    int numlinhas = numlinhasarq(fp);

    printf("linhas: %d\n", numlinhas);

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
            printf("\033[1;31mErro na linha %d: comando invalido.\n\033[0m", conta_linha);
            exit(1);
        }

        // verifica se a linha tem 2 ou 4 argumentos
        int numargs = 0;
        for (int k = 0; k < MAX; k++)
        {
            if (linha[k] == ' ' || linha[k] == '\n')
            {
                numargs++;
            }
        }

        numargs -= 1;

        if (numargs != 1 && numargs != 3)
        {
            if (numlinhas == conta_linha)
                printf("\033[1;31mErro na linha %d: numero de argumentos invalido. (%d) \033[0;31mVoce colocou um \\n? Se nao, coloque\n\033[0m", conta_linha, numargs);
            else
                printf("\033[1;31mErro na linha %d: numero de argumentos invalido. (%d)\n\033[0m", conta_linha, numargs);
            exit(1);
        }

        // verifica se o nome do programa é valido
        char *nomeprog = strtok(linha, " \n");
        nomeprog = strtok(NULL, " \n");

        strcpy(nomeprocesso, nomeprog);

        if (nomeprog == NULL)
        {
            printf("\033[1;31mErro na linha %d: nome do programa invalido.\n\033[0m", conta_linha);
            exit(1);
        }

        // verifica se o nome do programa tem no maximo 10 caracteres
        if (strlen(nomeprog) > 10)
        {
            printf("\033[1;31mErro na linha %d: nome do programa invalido.\n\033[0m", conta_linha);
            exit(1);
        }

        // verifica se o nome do programa é valido
        if (strncmp(nomeprog, "Run", 3) == 0)
        {
            printf("\033[1;31mErro na linha %d: nome do programa nao pode ser Run.\n\033[0m", conta_linha);
            exit(1);
        }
        // verifica se inclui os argumentos I e D
        if (numargs == 3)
        {
            char *inicio = strtok(NULL, " \n");
            char *duracao = strtok(NULL, " \n");

            if (inicio == NULL || duracao == NULL)
            {
                printf("\033[1;31mErro na linha %d: argumentos invalidos (%d).\n\033[0m", conta_linha, numargs);
                exit(1);
            }

            // verifica se o argumento I é valido
            if (strncmp(inicio, "I=", 2) != 0)
            {
                printf("\033[1;31mErro na linha %d: argumento I nao existe.\n\033[0m", conta_linha);
                exit(1);
            }

            // verifica se o argumento D é valido
            if (strncmp(duracao, "D=", 2) != 0)
            {
                printf("\033[1;31mErro na linha %d: argumento D nao existe.\n\033[0m", conta_linha);
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
                    printf("\033[1;31mErro na linha %d: argumento I precisa ser um numero.\n\033[0m", conta_linha);
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
                    printf("\033[1;31mErro na linha %d: argumento D precisa ser um numero.\n\033[0m", conta_linha);
                    exit(1);
                }
            }

            argd = atoi(duracao + 2);

            // verifica se o argumento I é valido
            int inicio_int = atoi(inicio + 2);
            if (inicio_int <= 0 || inicio_int >= 60)
            {
                printf("\033[1;31mErro na linha %d: argumento I nao pode ser negativo nem maior que 60.\n\033[0m", conta_linha);
                exit(1);
            }

            // verifica se o argumento D é valido
            int duracao_int = atoi(duracao + 2);
            if (duracao_int <= 0 || duracao_int >= 60)
            {
                printf("\033[1;31mErro na linha %d: argumento D nao pode ser negativo nem maior que 60.\n\033[0m", conta_linha);
                exit(1);
            }

            // verifica se a soma de I e D é maior que 60

            if (inicio_int + duracao_int > 60)
            {
                printf("\033[1;31mErro na linha %d: i + d não pode ser maior que 60.\n\033[0m", conta_linha);
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

        processos[i].executando = false;
        processos[i].criado = true;

        i++; // proximo processo

        printf("\033[1;34mProcesso %s adicionado.\n\033[0m", nomeprocesso);

        sleep(1); // delay de 1 segundo para enviar 1 por 1
    }

    printf("\033[1;32mTodos os processos foram adicionados.\n\033[0m");
    fclose(fp);

    return;
}