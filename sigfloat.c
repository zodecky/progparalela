#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void sigfpe_handler(int signum) {
    printf("tentou dividir por 0!\n");
    exit(1);
}

int main() {
    float a, b;
    int c, d;

    signal(SIGFPE, sigfpe_handler);

    // ler 2 numeros reais
    printf("Digite dois numeros float: ");
    scanf("%f %f", &a, &b);

    // Perform the four basic arithmetic operations on the real numbers
    printf("Operacoes:\n");
    printf("%f + %f = %f\n", a, b, a + b);
    printf("%f - %f = %f\n", a, b, a - b);
    printf("%f * %f = %f\n", a, b, a * b);
    printf("%f / %f = %f\n", a, b, a / b);


    // Le 2 numeros inteiros
    printf("Digite dois numeros inteiros: ");
    scanf("%d %d", &c, &d);

    // agora com os inteiros
    printf("Operacoes:\n");
    printf("%d + %d = %d\n", c, d, c + d);
    printf("%d - %d = %d\n", c, d, c - d);
    printf("%d * %d = %d\n", c, d, c * d);
    printf("%d / %d = %d\n", c, d, c / d);

    return 0;
}