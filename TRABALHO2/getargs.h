typedef struct args
{
    char algoritmo[10];
    char input[10];
    char pagsize[10];
    char memsize[10];
} args_t ;

args_t getargs(int argc, const char *argv[]);
void printargs(args_t args);