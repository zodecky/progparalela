#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SIZEVEC 100000

clock_t min(clock_t *listaclock)
{
    clock_t min = listaclock[0];
    for (int i = 1; i < 10; i++)
    {
        if (listaclock[i] < min)
        {
            min = listaclock[i];
        }
    }
    return min;
}

clock_t max(clock_t *listaclock)
{
    clock_t max = listaclock[0];
    for (int i = 1; i < 10; i++)
    {
        if (listaclock[i] > max)
        {
            max = listaclock[i];
        }
    }
    return max;
}

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

    int segmento2;
    int segmento3;

    segmento2 = shmget(IPC_PRIVATE, 10 * sizeof(clock_t), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    segmento3 = shmget(IPC_PRIVATE, 10 * sizeof(clock_t), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    clock_t *listaclockstart = (clock_t *)shmat(segmento2, 0, 0);
    clock_t *listaclockend = (clock_t *)shmat(segmento3, 0, 0);

    for (int i = 0; i < 10; i++)
    {
        pid_t pid = fork();
        listaclockstart[i] = clock();
        if (pid == 0)
        {
            int start = i * SIZEVEC / 10;
            int end = start + (SIZEVEC / 10 - 1);
            for (int j = start; j <= end; j++)
            {
                vetor5[j] = vetor5[j] * 2;
            }
            listaclockend[i] = clock();
            exit(0);
        }
    }

    start = min(listaclockstart);
    end = max(listaclockend);

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
