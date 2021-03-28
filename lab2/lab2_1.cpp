#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

struct targs
{
    int flag;
};

pthread_mutex_t m;

void * f1(void *arg)
{
    targs *args = (targs *) arg;
    int r;
    
    while(args->flag == 0)
    {
    	r = pthread_mutex_lock(&m);
    	if (r != 0)
    	{
    		printf("f1 pthread_mutex_lock error: %s\n", strerror(r));
    	}

    	for (int i = 0; i < 5; i++)
        {
            putchar('1');
        	fflush(stdout);
        	sleep(1);
        }
        
        r = pthread_mutex_unlock(&m);
        if (r != 0)
    	{
    		printf("f1 pthread_mutex_unlock error: %s\n", strerror(r));
    	}
        sleep(1);
    }
}

void * f2(void *arg)
{
    targs *args = (targs *) arg;
    int r;
    
    while(args->flag == 0)
    {
    	r = pthread_mutex_lock(&m);
    	if (r != 0)
    	{
    		printf("f2 pthread_mutex_lock error: %s\n", strerror(r));
    	}

    	for (int i = 0; i < 5; i++)
        {
            putchar('2');
        	fflush(stdout);
        	sleep(1);
        }
        
        r = pthread_mutex_unlock(&m);
        if (r != 0)
    	{
    		printf("f2 pthread_mutex_unlock error: %s\n", strerror(r));
    	}
        sleep(1);
    }
}

int main()
{
    targs arg1;
    targs arg2;
    arg1.flag = 0;
    arg2.flag = 0;

    int r;
    pthread_t t1;
    pthread_t t2;

    r = pthread_mutex_init(&m, NULL);
    if (r != 0)
    {
        printf("pthread_mutex_init error: %s", strerror(r));
        printf("\n");
    }

    r = pthread_create(&t1, NULL, f1, &arg1);
    if (r != 0)
    {
        printf("t1 pthread_create error: %s", strerror(r));
        printf("\n");
    }
    r = pthread_create(&t2, NULL, f2, &arg2);
    if (r != 0)
    {
        printf("t2 pthread_create error: %s", strerror(r));
        printf("\n");
    }

    getchar();

    arg1.flag = 1;
    arg2.flag = 1;

    r = pthread_join(t1, NULL);
    if (r != 0)
    {
        printf("pthread_join error: %s", strerror(r));
        printf("\n");
    }
    r = pthread_join(t2, NULL);
    if (r != 0)
    {
        printf("pthread_join error: %s", strerror(r));
        printf("\n");
    }

    r = pthread_mutex_destroy(&m);
    if (r != 0)
    {
        printf("pthread_mutex_destroy error: %s", strerror(r));
        printf("\n");
    }
    
    return 0;
}