#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define SIZEVEC 10000
#define NUMTHREADS 2

int vec[SIZEVEC];

void *worker(void *arg)
{
    long tid = (long)arg;

    for (int j = 0; j < SIZEVEC; j++)
    {
        vec[j] = vec[j] * 2;
        vec[j] = vec[j] + 2;
    }

    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid;
    
    int i;

    for (i = 0; i < SIZEVEC; i++)
    {
        vec[i] = 5;
    }

    // cria 2 threads que multiplicam por 2 todos elementos do vetor
    for (i = 0; i < NUMTHREADS; i++)
    {
        pthread_create(&tid, NULL, worker, (void *)i);
    }

    // espera as threads terminarem
    for (i = 0; i < NUMTHREADS; i++)
    {
        pthread_join(tid, NULL);
    }

    // imprime o vetor
    for (i = 0; i < SIZEVEC; i++)
    {
        printf("%d-", vec[i]);
    }


    return 0;
}