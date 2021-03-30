#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctime>

struct targs
{
    int flag;
};

int pipefd[2];

void * get_buffer(char **buffer)
{
    time_t rawtime = time(0);
    tm *lt = localtime(&rawtime);
    strftime(*buffer, sizeof(buffer), "%Y.%m.%d %H:%M:%S", lt);
    printf("%s\n", *buffer);
}

void * f1(void *arg)
{
    targs *args = (targs *) arg;
    int r;
    
    while(args->flag == 0)
    {
    	
        sleep(1);
    }
}

void * f2(void *arg)
{
    targs *args = (targs *) arg;
    int r;
    
    while(args->flag == 0)
    {
    	

    	for (int i = 0; i < 5; i++)
        {
            putchar('2');
        	fflush(stdout);
        	sleep(1);
        }
        
        
        sleep(1);
    }
}

int main()
{
    char buffer [80];
    get_buffer(&buffer);
    printf("%s\n", buffer);

    // targs arg1;
    // targs arg2;
    // arg1.flag = 0;
    // arg2.flag = 0;

    // int r;
    // pthread_t t1;
    // pthread_t t2;

    // r = pthread_mutex_init(&m, NULL);
    // if (r != 0)
    // {
    //     printf("pthread_mutex_init error: %s", strerror(r));
    //     printf("\n");
    // }

    // r = pthread_create(&t1, NULL, f1, &arg1);
    // if (r != 0)
    // {
    //     printf("t1 pthread_create error: %s", strerror(r));
    //     printf("\n");
    // }
    // r = pthread_create(&t2, NULL, f2, &arg2);
    // if (r != 0)
    // {
    //     printf("t2 pthread_create error: %s", strerror(r));
    //     printf("\n");
    // }

    // getchar();

    // arg1.flag = 1;
    // arg2.flag = 1;

    // r = pthread_join(t1, NULL);
    // if (r != 0)
    // {
    //     printf("pthread_join error: %s", strerror(r));
    //     printf("\n");
    // }
    // r = pthread_join(t2, NULL);
    // if (r != 0)
    // {
    //     printf("pthread_join error: %s", strerror(r));
    //     printf("\n");
    // }

    // r = pthread_mutex_destroy(&m);
    // if (r != 0)
    // {
    //     printf("pthread_mutex_destroy error: %s", strerror(r));
    //     printf("\n");
    // }
    
    return 0;
}