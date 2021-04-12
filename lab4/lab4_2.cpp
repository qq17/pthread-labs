#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    printf("lab4_1 argc: %d\n", argc);
    printf("lab4_1 arguments: \n");
    for (int i=1; i<argc; i++)
    {
        printf("%s\n", argv[i]);
        sleep(1);
    }

    printf("lab4_1 id: %d\n", getpid());
    printf("lab4_1 parent id: %d\n", getppid());

    return 0;
}