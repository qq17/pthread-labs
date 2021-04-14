#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        printf("lab4_1 was called with arguments: \n");
        for (int i = 1; i < argc; ++i)
        {
            printf("%s\n", argv[i]);
            sleep(1);
        }
    }
    else
    {
        printf("lab4_1 was called with no arguments\n");
    }

    printf("lab4_1 process id: %d\n", getpid());
    printf("lab4_1 parent process id: %d\n", getppid());
    printf("lab4_1 environment variable LANGUAGE: %s\n", getenv("LANGUAGE"));

    exit(55);
}