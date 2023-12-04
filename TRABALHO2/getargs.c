#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "getargs.h"

args_t getargs(int argc, const char *argv[])
{
    args_t args = {0, 0, 0, 0};
    int opt;

    if (argc == 1)
    {
        printf("\033[1;31mUso: %s -a <algoritmo> -i <arquivo input> -p <tamanho da pagina> -m <tamanho da memoria>\n\033[0m", argv[0]);
        exit(EXIT_FAILURE);
    }

    while ((opt = getopt(argc, argv, "a:i:p:m:")) != -1)
    {
        switch (opt)
        {
        case 'a': // algoritmo
        {
            strcpy(args.algoritmo, optarg);
            break;
        }
        case 'i': // arquivo input
        {
            strcpy(args.input, optarg);
            break;
        }
        case 'p': // tamanho da pagina
        {
            strcpy(args.pagsize, optarg);
            break;
        }
        case 'm': // tamanho da memoria
        {
            strcpy(args.memsize, optarg);
            break;
        }
        default:
            printf("\033[1;31mUso: %s -a <algoritmo> -i <arquivo input> -p <tamanho da pagina> -m <tamanho da memoria>\n\033[0m", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    return args;
}

void printargs(args_t args)
{
    printf("\033[1;34mAlgoritmo: %s\n", args.algoritmo);
    printf("Arquivo input: %s\n", args.input);
    printf("N paginas: %s\n", args.pagsize);
    printf("Tamanho memoria: %s\n\033[0m", args.memsize);
}
