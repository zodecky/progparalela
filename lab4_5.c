#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <signal.h>

int main(void) {
    pid_t pid1, pid2;
    int status;

    // cria os processos filhos 
    pid1 = fork();

    if (pid1 != 0) // evitar ter filhos demais
        pid2 = fork();

    if (pid1 == 0)
    {
        execvp("./print0", NULL);
    }

    if (pid2 == 0)
    {
        execvp("./print1", NULL);
    }

    int n = 0;

    while (n < 4)
    {
        kill(pid1, SIGCONT);
        sleep(1);
        kill(pid1, SIGSTOP);
        kill(pid2, SIGCONT);
        sleep(1);
        kill(pid2, SIGSTOP);
        n++;
    }

    kill(pid1, SIGKILL);
    kill(pid2, SIGKILL);

    return 0;
}