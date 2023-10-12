#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define EVER \
    ;        \
    ;

void childhandler(int signo);
int delay;
int main(int argc, char *argv[])
{
    pid_t pid;
    signal(SIGCHLD, childhandler);
    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "Erro ao criar filho\n");
        exit(-1);
    }
    if (pid == 0) /* filho */
        execvp("./sleep5", argv);
    else      /* pai */
    {
        scanf("%d", &delay); /* le delay da linha de comando */
        sleep(delay);
        printf("Programa %s excedeu o tempo limite de %d segundos.\n", argv[2], delay);
        kill(pid, SIGKILL);
        exit(0);
    }
    return 0;
}
void childhandler(int signo) /* Executado se o filho morre antes do pai */
{
    int status;
    pid_t pid = wait(&status);
    printf("Filho %d terminou dentro de %d segundos com status %d.\n", pid, delay, status);
    exit(0);
}