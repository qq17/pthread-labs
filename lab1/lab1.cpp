#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

using namespace std;

struct targs
{
    int flag;
};

void * f1(void *arg)
{
    targs *args = (targs *) arg;
    int r;

    while(args->flag == 0)
    {
        r = putchar('1');
        if (r == EOF)
        {
            perror("putchar error: ");
        }
        r = fflush(stdout);
        if (r == EOF)
        {
            perror("fflush error: ");
        }
        r = sleep(1);
        if (r != 0)
        {
            perror("sleep error: ");
        }
    }
    pthread_exit((void *)55);
}

void * f2(void *arg)
{
    targs *args = (targs *) arg;
    int r;

    while(args->flag == 0)
    {
        r = putchar('2');
        if (r == EOF)
        {
            perror("putchar error: ");
        }
        r = fflush(stdout);
        if (r == EOF)
        {
            perror("fflush error: ");
        }
        r = sleep(1);
        if (r != 0)
        {
            perror("sleep error: ");
        }
    }
    pthread_exit((void *)77);
}

int main()
{
    targs arg1;
    targs arg2;
    arg1.flag = 0;
    arg2.flag = 0;
    int r1;
    int r2;
    int r;
    pthread_t t1;
    pthread_t t2;

    r = pthread_create(&t1, NULL, f1, &arg1);
    if (r != 0)
    {
        printf("pthread_create error: %s", strerror(r));
    }
    r = pthread_create(&t2, NULL, f2, &arg2);
    if (r != 0)
    {
        printf("pthread_create error: %s", strerror(r));
    }

    r = getchar();
    if (r == EOF)
    {
        perror("getchar error: ");
    }

    arg1.flag = 1;
    arg2.flag = 1;

    r = pthread_join(t1, (void **)&r1);
    if (r != 0)
    {
        printf("pthread_join error: %s", strerror(r));
    }
    r = pthread_join(t2, (void **)&r2);
    if (r != 0)
    {
        printf("pthread_join error: %s", strerror(r));
    }

    r = printf("exitcode from t1 = %d\n", r1);
    if (r < 0)
    {
        perror("printf error: ");
    }
    r = printf("exitcode from t2 = %d\n", r2);
    if (r < 0)
    {
        perror("printf error: ");
    }
    
    return 0;
}
