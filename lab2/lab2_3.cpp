#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

pthread_mutex_t m;

void * f1(void *fl1)
{
    struct timespec t;
    
    while(*(int *)fl1 == 0)
    {
        clock_gettime(CLOCK_REALTIME, &t);
        t.tv_sec += 1;

        pthread_mutex_timedlock(&m, &t);
        for (int i = 0; i<5; i++)
        {
            putchar('1');
            fflush(stdout);
            sleep(1);
        }
        pthread_mutex_unlock(&m);
        sleep(1);
    }
}

void * f2(void *fl2)
{
    struct timespec t;

    while(*(int *)fl2 == 0)
    {
        clock_gettime(CLOCK_REALTIME, &t);
        t.tv_sec += 1;

        pthread_mutex_lock(&m);
        for (int i = 0; i<5; i++)
        {
            putchar('2');
            fflush(stdout);
            sleep(1);
        }
        pthread_mutex_unlock(&m);
        sleep(1);
    }
}

int main()
{
    int fl1 = 0;
    int fl2 = 0;
    
    pthread_t t1;
    pthread_t t2;

    pthread_mutex_init(&m, NULL);

    pthread_create(&t1, NULL, f1, &fl1);
    pthread_create(&t2, NULL, f2, &fl2);
    getchar();

    fl1 = 1;
    fl2 = 1;
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    

    return 0;
}
