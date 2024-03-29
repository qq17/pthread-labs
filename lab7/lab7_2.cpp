#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 80

struct targs
{
    int flag;
};

int fifod;
char fifo_name[] = "/tmp/lab7_fifo";

void * thread_func(void *arg)
{
    targs *args = (targs *) arg;
    char buffer[BUF_SIZE];
    int r;

    while(args->flag == 0)
    {
        memset(buffer, '\0', BUF_SIZE);
        r = read(fifod, buffer, BUF_SIZE);
        if (r == -1)
        {
            perror("lab7_2 read error");
            sleep(1);
        }
        else if (r == 0)
        {
            printf("EOF, writing program is closed\n");
            sleep(1);
        }
        else
        {
            printf("read: %s\n", buffer);
        }
    }
    pthread_exit((void *)0);
}

int main()
{
    targs arg;
    arg.flag = 0;
    pthread_t th_func;
    int r;
    int exitcode;

    r = mkfifo(fifo_name, 0644);
    if (r == -1)
    {
        perror("lab7_2 mkfifo error");
    }

    fifod = open(fifo_name, O_RDONLY | O_NONBLOCK);
    if (fifod == -1)
    {
        perror("lab7_2 open error");
    }

    r = pthread_create(&th_func, NULL, thread_func, &arg);
    if (r != 0)
    {
        printf("lab7_2 pthread_create error: %s", strerror(r));
        printf("\n");
    }

    getchar();

    arg.flag = 1;

    r = pthread_join(th_func, (void **)&exitcode);
    if (r != 0)
    {
        printf("lab7_2 pthread_join error: %s", strerror(r));
        printf("\n");
    }
    else
    {
        printf("lab7_2 exitcode from thread_func: %d\n", exitcode);
    }

    close(fifod);
    if (r == -1)
    {
        perror("lab7_2 close error");
        return 1;
    }

    unlink(fifo_name);
    if (r == -1)
    {
        perror("lab7_2 unlink error");
        return 2;
    }
    
    return 0;
}