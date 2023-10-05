#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void sigint_handler(int signum) {
    printf("Pressionou ctrl+c\n");
}

void sigquit_handler(int signum) {
    printf("Pressionou ctrl+\\, finalizando...\n");
    exit(0);
}

int main() {
    // Registra o handler para SIGINT
    signal(SIGINT, sigint_handler);

    // Registra o handler para SIGQUIT
    signal(SIGQUIT, sigquit_handler);

    // Espera por um SIGINT
    printf("Pressione ctrl+c para causar um SIGINT\n");
    while (1);
    return 0;
}