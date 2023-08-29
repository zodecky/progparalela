#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    iniloop:
    while (1)
    {
        printf("Gabishell> ");
        char *linha = NULL;
        size_t len = 0;
        ssize_t read = getline(&linha, &len, stdin); // lê a linha digitada pelo usuário
        if (read == -1) // caso erro
        {
            printf("Erro lendo a linha \n");
            exit(1);
        }
        else
        {


            // caso o usuário digite apenas enter, reinicia o loop
            if (strcmp(linha, "\n") == 0)
            {
                goto iniloop;
            }

            // remove o \n do final da linha
            if (linha[read - 1] == '\n')
            {
                linha[read - 1] = '\0';
            }
            

            char *token = strtok(linha, " ");
            char *args[100]; // vetor de argumentos
            int i = 0;
            while (token != NULL) // separa a linha em tokens
            {
                args[i++] = token; // adiciona o token no vetor de argumentos
                token = strtok(NULL, " "); // NULL significa que vai continuar a partir do último token
            }
            args[i] = NULL; // o último argumento deve ser NULL

            if (strcmp(args[0], "exit") == 0) // se o usuário digitar exit, sai do programa (comando customizado)
            {
                exit(0);
            }
            else
            {
                pid_t pid = fork(); // Inicia um multiprocesso para executar o comando e nao sair do programa
                if (pid == 0) // processo filho
                {
                    execvp(args[0], args); // <-- caso sucesso exit acontece aqui 
                    printf("Erro executando o comando\n"); // caso erro
                    exit(1); // mata o processo filho, retorna codigo de erro
                }
                else // processo pai
                {
                    wait(NULL); // espera o processo filho terminar
                }
            }


        }
    }
}