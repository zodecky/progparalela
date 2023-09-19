#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{


    //memoria compartilhada
    int segmento;
    char *mensagem;

    // aloca a memória compartilhada
    segmento = shmget(7000, 100, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    // associa a memória compartilhada ao processo
    mensagem = (char *)shmat(segmento, 0, 0);
    printf("%s", mensagem);

    shmdt(mensagem);
    shmctl(segmento, IPC_RMID, 0);

    return 0;
}
