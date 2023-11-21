#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHM_KEY 7000
#define MAX_SIZE 1024

int main() {
    int shmid = shmget(SHM_KEY, MAX_SIZE, 0644);
    char *data = shmat(shmid, NULL, 0);

    printf("Mensagem do dia: %s\n", data);

    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}