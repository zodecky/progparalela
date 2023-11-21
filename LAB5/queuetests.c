#include "stringqueue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    Queue *queue = createQueue();


    mensagem msg1 = {"string 1"};
    mensagem msg2 = {"string 2"};
    mensagem msg3 = {"string 3"};
    mensagem msg4 = {"string 4"};
    mensagem msg5 = {"string 5"};
    mensagem msg6 = {"string 6"};
    mensagem msg7 = {"string 7"};
    mensagem msg8 = {"string 8"};
    mensagem msg9 = {"string 9"};
    mensagem msg10 = {"string 10"};


    enqueue(queue, msg1);
    enqueue(queue, msg2);
    enqueue(queue, msg3);
    enqueue(queue, msg4);
    enqueue(queue, msg5);
    enqueue(queue, msg6);
    enqueue(queue, msg7);
    enqueue(queue, msg8);
    enqueue(queue, msg9);
    enqueue(queue, msg10);

    print(queue);

    printf("dequeue: %s\n", dequeue(queue).str);
    printf("dequeue: %s\n", dequeue(queue).str);
    printf("dequeue: %s\n", dequeue(queue).str);
    printf("dequeue: %s\n", dequeue(queue).str);
    printf("dequeue: %s\n", dequeue(queue).str);
    printf("dequeue: %s\n", dequeue(queue).str);
    printf("dequeue: %s\n", dequeue(queue).str);
    printf("dequeue: %s\n", dequeue(queue).str);
    printf("dequeue: %s\n", dequeue(queue).str);
    printf("dequeue: %s\n", dequeue(queue).str);
    printf("dequeue: %s (NULL)\n", dequeue(queue).str);

}