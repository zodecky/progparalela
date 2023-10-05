#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

// kill -s KILL <numprocesso (pid)> em outra janela

void sigkill_handler(int signum) {
    printf("SIGKILL recebido!\n");
    exit(1);
}

int main(void)
{
    signal(SIGKILL, sigkill_handler);
    printf("SIGKILL handler definido!\n");
    printf("pid: %d\n", getpid());
    while(1);
    return 0;
}