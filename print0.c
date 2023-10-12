#include <stdio.h>
#include <stdbool.h>

int main(int argc, char const *argv[])
{
    while (true)
    {
        for (int i = 0; i < 1000000; i++)
        {
            if (i % 100000 == 0)
            {
                printf("0");
            }
        }
    }
    return 0;
}
