#include "getargs.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    printf("argc: %d\n", argc);
    printargs(getargs(argc, argv));

    
    return 0;
}
