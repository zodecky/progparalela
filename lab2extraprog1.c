#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>

#define SHM_KEY 7000
#define MAX_SIZE 1024

int main() {
    char message[MAX_SIZE];

    printf("Digite a mensagem do dia: ");
    fgets(message, MAX_SIZE, stdin);

    int shmid = shmget(SHM_KEY, MAX_SIZE, 0644 | IPC_CREAT);
    char *data = shmat(shmid, NULL, 0);

    strncpy(data, message, MAX_SIZE);

    shmdt(data);

    return 0;
}