// Gabriel Zagury de Magalhães - 2210912
// Elisa Vignoud - 2320149

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(void)
{
    pid_t pid;
    int n = 3;


    pid = fork();
    if (pid == -1)
    {
        return (1);
    }

    if (pid == 0) // processo filho
    {
        for (int i = 0; i < 10000; i++, n += 10)
        {
            printf("processo filho, pid=%d, n=%d\n", getpid(), n);
        }
        exit(0); // termina o processo filho
    }
    else
    {
        for (int i = 0; i < 10000; i++, n++)
        {
            printf("processo pai, pid=%d, n=%d\n", getpid(), n);
        }
        wait(NULL); // espera o filho terminar
    }
    return(0);
}