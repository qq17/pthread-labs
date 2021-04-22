#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctime>

#define BUF_SIZE 80

struct targs
{
    int flag;
};

int fifod;
pthread_t th_open;
pthread_t th_func;

void * thread_func(void *arg)
{
    targs *args = (targs *) arg;
    int r;

    char buffer[BUF_SIZE];
    time_t rawtime;
    tm *loctime;

    while(args->flag == 0)
    {
        rawtime = time(0);
        loctime = localtime(&rawtime);
        r = strftime(buffer, BUF_SIZE, "%Y.%m.%d %H:%M:%S", loctime);

        r = write(fifod, buffer, r);
        if (r == -1)
        {
            perror("lab7_1 write error");
        }

        sleep(1);
    }
    pthread_exit((void *)0);
}

void * thread_open(void *arg)
{
    targs *args = (targs *) arg;
    char fifo_name[] = "/tmp/lab7_fifo";
    int r;

    while(args->flag == 0)
    {
        fifod = open(fifo_name, O_WRONLY | O_NONBLOCK);
        if (fifod == -1)
        {
            perror("lab7_1 open error");
            sleep(1);
        }
        else
        {
            r = pthread_create(&th_func, NULL, thread_func, &arg);
            if (r != 0)
            {
                printf("lab7_1 pthread_create error: %s", strerror(r));
                printf("\n");
                pthread_exit((void *)1);
            }
            pthread_exit((void *)0);
        }
    }
    pthread_exit((void *)0);
}

int main()
{
    targs arg;
    arg.flag = 0;
    int r;
    int exitcode;
    char fifo_name[] = "/tmp/lab7_fifo";

    r = mkfifo(fifo_name, 0644);
    if (r == -1)
    {
        perror("lab7_1 mkfifo error");
    }

    r = pthread_create(&th_open, NULL, thread_open, &arg);
    if (r != 0)
    {
        printf("lab7_1 pthread_create error: %s", strerror(r));
        printf("\n");
    }

    getchar();

    arg.flag = 1;

    r = pthread_join(th_open, (void **)&exitcode);
    if (r != 0)
    {
        printf("lab7_1 pthread_join error: %s", strerror(r));
        printf("\n");
    }

    printf("lab7_1 exitcode from thread_open: %d\n", exitcode);

    r = pthread_join(th_func, (void **)&exitcode);
    if (r != 0)
    {
        printf("lab7_1 pthread_join error: %s", strerror(r));
        printf("\n");
    }
    else
    {
        printf("lab7_1 exitcode from thread_func: %d\n", exitcode);
    }

    close(fifod);
    if (r == -1)
    {
        perror("lab7_1 close error");
        return 13;
    }

    unlink(fifo_name);
    if (r == -1)
    {
        perror("lab7_1 unlink error");
        return 14;
    }
    
    return 0;
}