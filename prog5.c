#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    while (true)
    {
        printf("prog5: pid=%d\n", getpid());
        sleep(1);
    }
    return 0;
}