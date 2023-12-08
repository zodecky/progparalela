#ifndef GETARGS_H
#define GETARGS_H

typedef struct args
{
    char algoritmo[10];
    char input[50];
    int pagsize;
    int memsize;
} args_t ;

args_t getargs(int argc, const char *argv[]);
void printargs(args_t args);
#endif