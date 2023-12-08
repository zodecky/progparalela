#include "getargs.h"
#include "pages.h"
#include "relatorio.h"
#include <stdio.h>

void printrelatorio(relatorio_t relatorio)
{
    printf("\033[0;32m"); 
    printf("\n\n********************\n\nRelatorio:\n");
    printf("Resets do timer: %d\n", relatorio.resets_do_timer);
    printf("Page faults: %d\n", relatorio.page_faults);
    printf("Paginas escritas: %d\n", relatorio.paginas_escritas);
    printf("\033[0m"); 
}

int main(int argc, char const *argv[])
{
    printf("argc: %d\n", argc);
    args_t args = getargs(argc, argv);
    printargs(args);

    relatorio_t relatorio = le_arquivo(args);

    printrelatorio(relatorio);
    
    return 0;
}
