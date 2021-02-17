#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

struct targs
{
    int flag;
};

void * f1(void *arg)
{
    targs *args = (targs *) arg;

    while(args->flag == 0)
    {
        putchar('1'); //check
        fflush(stdout); //check
        sleep(1); //check
    }
    pthread_exit((void *)55); //check
}

void * f2(void *arg)
{
    targs *args = (targs *) arg;
    
    while(args->flag == 0)
    {
        putchar('2'); //check
        fflush(stdout); //check
        sleep(1); //check
    }
    pthread_exit((void *)77); //check
}

int main()
{
    targs arg1;
    targs arg2;
    arg1.flag = 0;
    arg2.flag = 0;
    int r1;
    int r2;
    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1, NULL, f1, &arg1); // check
    pthread_create(&t2, NULL, f2, &arg2); // check
    getchar(); //check

    arg1.flag = 1;
    arg2.flag = 1;
    pthread_join(t1, (void **)&r1); //check
    pthread_join(t2, (void **)&r2); //check
    printf("exitcode from t1 = %d\n", r1); //check
    printf("exitcode from t2 = %d\n", r2); //check
    
    return 0;
}
