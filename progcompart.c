#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{


    // leia da entrada uma mensagem do dia
    char *msg = "Mensagem de teste\n";
    scanf("%s", msg);

    //memoria compartilhada
    int segmento;
    char *mensagem;

    // aloca a memória compartilhada
    segmento = shmget(7001, 100, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    // associa a memória compartilhada ao processo
    mensagem = (char *)shmat(segmento, 0, 0);
    strcpy(mensagem, msg);

    return 0;
}
