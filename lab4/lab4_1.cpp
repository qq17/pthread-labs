#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        printf("lab4_1 was called with arguments: \n");
        for (int i=1; i<argc; i++)
        {
            printf("%s\n", argv[i]);
            sleep(1);
        }
    }
    else
    {
        printf("lab4_1 was called with no arguments\n");
    }

    printf("lab4_1 id: %d\n", getpid());
    printf("lab4_1 parent id: %d\n", getppid());

    exit(0);
}