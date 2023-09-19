#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SIZEVEC 100000

int main(void)
{
    clock_t start, end;
    double cpu_time_used;

    // ************ PROGRAMA PARALELO *************

    int segmento;

    segmento = shmget(IPC_PRIVATE, SIZEVEC * sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    int *vetor5 = (int *)shmat(segmento, 0, 0);

    for (int i = 0; i < SIZEVEC; i++)
    {
        vetor5[i] = 5;
    }

    start = clock();

    for (int i = 0; i < 10; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            int start = i * SIZEVEC / 10;
            int end = start + (SIZEVEC / 10 - 1) ;
            for (int j = start; j <= end; j++)
            {
                vetor5[j] = vetor5[j] * 2;
            }
            exit(0);
        }
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Tempo de execução paralelo: %f\n", cpu_time_used);

    // for (int i = 0; i < SIZEVEC; i++)
    // {
    //     printf("%d\n", vetor5[i]);
    // }

    shmdt(vetor5);

    shmctl(segmento, IPC_RMID, 0);

    // ************ PROGRAMA LINEAR *************

    int vetor[SIZEVEC];

    for (int i = 0; i < SIZEVEC; i++)
    {
        vetor[i] = 5;
    }

    start = clock();

    for (int i = 0; i < SIZEVEC; i++)
    {
        vetor[i] = vetor[i] * 2;
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Tempo de execução linear: %f\n", cpu_time_used);

    return 0;
}
