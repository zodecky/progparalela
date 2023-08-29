#include <stdio.h>

int main(int argc, const char *argv[])
{
    if (argc <= 1) // caso digito apenas cat
    {
        printf("Uso: %s <filename>\n", argv[0]);
        return 1;
    }
    else // caso normal
    {
        FILE *arquivo = fopen(argv[1], "r");
        int c;

        while ((c = fgetc(arquivo)) != EOF)
        {
            putc(c, stdout);
        }
    }
    return 0;
}