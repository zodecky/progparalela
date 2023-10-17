#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>

float time_diff(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec - start->tv_sec) + 1e-6 * (end->tv_usec - start->tv_usec);
}

int main(void) {
    pid_t pid1, pid2, pid3;
    int status;

    // cria os processos filhos 
    pid1 = fork();

    if (pid1 != 0) // evitar ter filhos demais
        pid2 = fork();

    if (pid1 != 0 && pid2 != 0)
        pid3 = fork();

    if (pid1 == 0)
    {
        execvp("./print0", 0);
    }

    if (pid2 == 0)
    {
        execvp("./print1", 0);
    }

    if (pid3 == 0)
    {
        execvp("./print2", 0);
    }

    struct timeval *loopstart = malloc(sizeof(struct timeval));
    struct timeval *loopatual = malloc(sizeof(struct timeval));

    gettimeofday(loopstart, NULL);

    kill(pid1, SIGSTOP);
    kill(pid2, SIGSTOP);
    kill(pid3, SIGSTOP);

    printf("\n\n---------valendo---------\n\n");
    sleep(1);

    bool once_a_sec = true;
    int t = 0;
    int ant = 0;
    float tempo_g = 0;

    while (true)
    {
        if (t != ant) // falso quem ativa é cada parte
            once_a_sec = true;


        if (tempo_g >= 120) // tempo p acabar
            break;

        if (t == 0 && once_a_sec) // tempo do minuto
        {
            kill(pid1, SIGSTOP);
            kill(pid2, SIGSTOP);
            kill(pid3, SIGCONT); // comeca executando p3
            once_a_sec = false;
        }

        // ******************* logica P1 *******************

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

        // ******************* logica P2 *******************

        if (t == 45 && once_a_sec)
        {
            kill(pid2, SIGCONT);
            once_a_sec = false;

            kill(pid3, SIGSTOP); // pausa 3 apos 45s
        } // para sozinho no 60


        // ******************* logica geral *******************
        gettimeofday(loopatual, NULL);
        tempo_g = time_diff(loopstart, loopatual);
        ant = t;
        t = (int)tempo_g % 60;

        //tudo embaixo para comecar executando p3 corretamente

    }

    kill(pid1, SIGKILL);
    kill(pid2, SIGKILL);
    kill(pid3, SIGKILL);

    free(loopstart);
    free(loopatual);

    return 0;
}