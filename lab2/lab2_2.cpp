#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

pthread_mutex_t m;

void * f1(void *fl1)
{
    while(*(int *)fl1 == 0)
    {
        pthread_mutex_trylock(&m);
        for (int i = 0; i<5; i++)
        {
            cout << "1" << endl;
            sleep(1);
        }
        pthread_mutex_unlock(&m);
        sleep(1);
    }
}

void * f2(void *fl2)
{
    while(*(int *)fl2 == 0)
    {
        pthread_mutex_trylock(&m);
        for (int i = 0; i<5; i++)
        {
            cout << "2" << endl;
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
