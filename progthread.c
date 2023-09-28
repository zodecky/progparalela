#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZEVEC 100000000
#define NUMTHREADS 2

int vector[SIZEVEC];
clock_t listastart[NUMTHREADS];
clock_t listaend[NUMTHREADS];

clock_t min(clock_t *listaclock)
{
    clock_t min = listaclock[0];
    for (int i = 1; i < NUMTHREADS; i++)
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
    for (int i = 1; i < NUMTHREADS; i++)
    {
        if (listaclock[i] > max)
        {
            max = listaclock[i];
        }
    }
    return max;
}

void *worker(void *arg)
{   
    long tid = (long)arg;

    long start = (long)arg * (SIZEVEC / NUMTHREADS);
    int end = start + (SIZEVEC / NUMTHREADS - 1);

    listastart[(long)arg] = clock();

    for (int j = start; j <= end; j++)
    {
        vector[j] = vector[j] * 2;
    }

    listaend[(long)arg] = clock();

    pthread_exit(NULL);
}

int main(void)
{
    clock_t start, end;
    double cpu_time_used;

    // ************ PROGRAMA PARALELO *************


    for (int i = 0; i < SIZEVEC; i++)
    { 
        vector[i] = 5;
    }

    pthread_t threads[NUMTHREADS]; // array de threads


    for (long i = 0; i < NUMTHREADS; i++) // cria o num de threads e passa o num dela
    {
        pthread_create(&threads[i], NULL, worker, (void *)i); // (endereco, atributos, funcao, argumento)
    } // para passar varios args usar ponteiro para struct com os args 


    for (int i = 0; i < NUMTHREADS; i++)
    {
        pthread_join(threads[i], NULL); // espera a thread terminar
    }


    start = min(listastart);
    end = max(listaend);

    
    // print listastart e listaend
    for (int i = 0; i < NUMTHREADS; i++)
    {
        printf("%ld - %ld\n", listastart[i], listaend[i]);
    }

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;


    // for (int i = 0; i < SIZEVEC; i++)
    // {
    //     printf("%d-", vector[i]);
    // }
    printf("Tempo de execução paralelo: %f\n", cpu_time_used);
    
    // ************ PROGRAMA LINEAR *************


    for (int i = 0; i < SIZEVEC; i++)
    {
        vector[i] = 5;
    }
    start = clock();

    for (int i = 0; i < SIZEVEC; i++)
    {
        vector[i] = vector[i] * 2;
    }

    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Tempo de execução linear: %f\n", cpu_time_used);

    return 0;



}