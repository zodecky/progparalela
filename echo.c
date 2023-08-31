// Gabriel Zagury de Magalhães - 2210912
// Elisa Vignoud - 2320149

#include <stdio.h>

int main(int argc, const char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        printf("%s ", argv[i] ? argv[i] : "");
    }
    printf("\n");
    return 0;
}

