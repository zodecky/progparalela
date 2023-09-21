#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SIZEVEC 10000

int main(void)
{
    // ************ PROGRAMA PARALELO *************

    int segmento;

    segmento = shmget(IPC_PRIVATE, SIZEVEC * sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    int *vetor5 = (int *)shmat(segmento, 0, 0);

    for (int i = 0; i < SIZEVEC; i++)
    {
        vetor5[i] = 5;
    }

    pid_t pid = fork(); // Cria um processo filho

    if (pid == 0)
    {
        // sleep(1);
        for (int i = 0; i < SIZEVEC; i++)
        {
            vetor5[i] = (vetor5[i] * 2) + 2;
        }
        exit(0);
    }
    pid_t pid2 = fork(); // Cria um processo filho

    if (pid2 == 0)
    {
        for (int i = 0; i < SIZEVEC; i++)
        {
            vetor5[i] = (vetor5[i] * 2) + 3;
        }
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    // print vec

    for (int i = 0; i < SIZEVEC; i++)
    {
        printf("%d-", vetor5[i]);
    }

    return 0;
}
