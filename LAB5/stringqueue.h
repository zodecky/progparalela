#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#include "config.h"

typedef struct {
    char str[MAX_SIZE];
} mensagem;

typedef struct {
    mensagem q[QUEUE_SIZE];
    int ini, fim;
    bool inicializada;
} Queue;

bool enqueue(Queue *q, mensagem msg), print(Queue *q);
mensagem dequeue(Queue *q);
bool isEmpty(Queue *q), isFull(Queue *q);
Queue *createQueue();
void freeQueue(Queue *q);
bool inicializada(Queue *q);

Queue *createQueue() {
    Queue *q = (Queue *) malloc(sizeof (Queue));
    q->ini = q->fim = 0;
    return q;
}

void freeQueue(Queue *q) {
    free(q);
}

bool enqueue(Queue *q, mensagem msg) {
    if (isFull(q)) {
        printf("Queue overflow\n");
        return false;
    }
    q->q[q->fim] = msg;
    q->fim = (q->fim + 1) % QUEUE_SIZE;
    return true;
}

mensagem dequeue(Queue *q) {
    mensagem msg;
    mensagem erro = {"erro"};
    if (isEmpty(q)) {
        printf("Queue underflow\n");
        return erro;
    }
    msg = q->q[q->ini];
    q->ini = (q->ini + 1) % QUEUE_SIZE;
    return msg;
}

bool isEmpty(Queue *q) {
    return (q->ini == q->fim);
}

bool isFull(Queue *q) {
    return (q->fim + 1) % QUEUE_SIZE == q->ini;
}

bool print(Queue *q) {
    int i;
    if (isEmpty(q)) {
        printf("Queue underflow\n");
        return false;
    }
    for (i = q->ini; i != q->fim; i = (i + 1) % QUEUE_SIZE)
        printf("%s\n", q->q[i].str);
    return true;
}

bool inicializada(Queue *q) {
    return q->inicializada;
}

