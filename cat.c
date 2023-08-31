// Gabriel Zagury de Magalhães - 2210912
// Elisa Vignoud - 2320149

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
        for (int i = 1; i < argc; i++) // para cada arquivo
        {
            FILE *arquivo = fopen(argv[i], "r");

            if (arquivo == NULL)
            {
                printf("Erro abrindo o arquivo %s\n", argv[i]);
                return 1;
            }

            int c;

            printf("Conteúdo do arquivo %s:\n\n", argv[i]);

            while ((c = fgetc(arquivo)) != EOF)
            {
                putc(c, stdout);
            }

            printf("\n\n");
        }
    }

    return 0;
}