#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

//int flag1 = 0;
//int flag2 = 0;

void * f1(void *fl1)
{
    while(*(int *)fl1 == 0)
    {
        cout << "1" << endl;
        sleep(1);
    }
    //int r [1] = {55};
    pthread_exit((void *)55);
    //return "st";
}

void * f2(void *fl2)
{
    while(*(int *)fl2 == 0)
    {
        cout << "2" << endl;
        sleep(1);
    }
    //int r [1] = {77};
    pthread_exit((void *)77);
    //return "ri";
}

int main()
{
    int fl1 = 0;
    int fl2 = 0;
    //int *r1;
    //int *r2;
    int r1;
    int r2;
    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1, NULL, f1, &fl1);
    pthread_create(&t2, NULL, f2, &fl2);
    getchar();

    fl1 = 1;
    fl2 = 1;
    pthread_join(t1, (void **)&r1);
    pthread_join(t2, (void **)&r2);
    cout << r1 << endl;
    cout << r2 << endl;
    //getchar();

    
    getchar();
    return 0;
}
