#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>


/**
 * @brief Função que calcula o tempo decorrido
 * 
 * @param start 
 * @param end 
 * @return float 
 */
float time_diff(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec - start->tv_sec) + 1e-6 * (end->tv_usec - start->tv_usec); // desencapsula estrutura
}

int main(void) {
    pid_t pid1, pid2, pid3; // cada um é um processo
    int status;

    // o primeiro processo filho
    pid1 = fork();

    if (pid1 != 0) // para evitar ter filhos demais
        pid2 = fork();

    if (pid1 != 0 && pid2 != 0) // apenas o pai pode criar filhos
        pid3 = fork();

    if (pid1 == 0) // envia cada
    {
        execvp("./print0", 0);
    }

    if (pid2 == 0) // processo filho
    {
        execvp("./print1", 0);
    }

    if (pid3 == 0) // para um programa diferente
    {
        execvp("./print2", 0);
    }

    // cria as estruturas
    struct timeval *loopstart = malloc(sizeof(struct timeval));
    struct timeval *loopatual = malloc(sizeof(struct timeval));

    gettimeofday(loopstart, NULL); // pega o tempo inicial

    kill(pid1, SIGSTOP); // pausa todos processos,
    kill(pid2, SIGSTOP); // para certificar que
    kill(pid3, SIGSTOP); // nenhum execute fora do momento aprorpiado

    printf("\n\n---------valendo (em 1s)---------\n\n"); // indica que começou
    sleep(1);

    bool once_a_sec = true; // permite que apenas uma operaçao seja feita por segundo
    int t = 0;              // tempo em um loop de 60
    int ant = 0;            // tempo anterior (para o once_a_sec)
    float tempo_g = 0;      // tempo global (sem retornar a 0 em 60)

    while (true)
    {
        if (t != ant) // se o segundo mudou
            once_a_sec = true; // uma nova operaçao pode ser feita


        if (tempo_g >= 120) // tempo p acabar (global)
            break;

        if (t == 0 && once_a_sec) // incio do minuto
        {
            kill(pid1, SIGSTOP);
            kill(pid2, SIGSTOP);
            kill(pid3, SIGCONT); // comeca executando p3
            once_a_sec = false;
        }

        // ******************* logica P1 e P3 *******************

        if (t == 5 && once_a_sec)
        {
            kill(pid1, SIGCONT);
            once_a_sec = false;

            kill(pid3, SIGSTOP); // pausa 3 apos 5s
        }

        if (t == 25 && once_a_sec)
        {
            kill(pid1, SIGSTOP);
            once_a_sec = false;

            kill(pid3, SIGCONT); // volta 3 apos 25s
        }

        // ******************* logica P2 e P3 *******************

        if (t == 45 && once_a_sec)
        {
            kill(pid2, SIGCONT);
            once_a_sec = false;

            kill(pid3, SIGSTOP); // pausa 3 apos 45s
        } // para sozinho no 60


        // ******************* logica geral *******************

        /*
        Aqui é calculado o tempo decorrido
        */

        gettimeofday(loopatual, NULL); 
        tempo_g = time_diff(loopstart, loopatual);
        ant = t;
        t = (int)tempo_g % 60;

        //tudo embaixo para comecar executando p3 corretamente

    }

    
    /*
        Ao acabar o tempo,
        mata os processos filhos e 
        libera a memoria.
    */

    kill(pid1, SIGKILL);
    kill(pid2, SIGKILL);
    kill(pid3, SIGKILL);

    free(loopstart);
    free(loopatual);

    return 0;
}