#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid;
    char *msg = "Mensagem de teste\n";

    //memoria compartilhada
    int segmento;
    char *mensagem;

    // aloca a memória compartilhada
    segmento = shmget(IPC_PRIVATE, 100, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    // associa a memória compartilhada ao processo
    

    // cria um processo filho
    pid = fork();

    if (pid == 0) // é o filho
    {
        // altera a mensagem   
        sleep(1);
        mensagem = (char *)shmat(segmento, 0, 0);
        strcpy(mensagem, "Mensagem alterada pelo filho\n");
        exit(0);
    }
    else // é o pai
    {
        mensagem = (char *)shmat(segmento, 0, 0);
        sleep(1);
        strcpy(mensagem, "Mensagem alterada pelo pai\n");
        printf("%s", mensagem);
        wait(NULL);
    }

    // libera a memória compartilhada do processo
    shmdt(mensagem);
    // liberar a memória compartilhada
    shmctl(segmento, IPC_RMID, 0);


    return 0;
}
