#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main()
{
    pid_t pid, status;
    int time = 0;
    pid = fork();
    if (pid == 0)
    {
        while (1)
        {
            printf("Tempo de execução do programa filho: %d segundos\n", time);
            sleep(1);
            time++;
        }
    }
    else
    {
        while (1)
        {
            sleep(2);
            kill(pid, SIGSTOP);
            sleep(2);
            kill(pid, SIGCONT);
        }
        wait(&status);
    }
    return 0;
}