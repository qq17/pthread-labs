#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <ctime>

#define BUF_SIZE 80

struct targs
{
    int flag;
};

mqd_t mq;
char mq_name[] = "/lab8_mq";

void sig_handler(int signo)
{
    printf("got SIGPIPE\n");
}

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

        r = mq_send(mq, buffer, r, 0);
        printf("send: %s\n", buffer);
        if (r == -1)
        {
            perror("lab8_1 mq_send error");
        }

        sleep(1);
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

    // signal(SIGPIPE, sig_handler);

    mq = mq_open(mq_name, O_CREAT | O_WRONLY | O_NONBLOCK, 0644, NULL);
    if (mq == (mqd_t)-1)
    {
        perror("lab8_1 mq_open error");
    }

    r = pthread_create(&th_func, NULL, thread_func, &arg);
    if (r != 0)
    {
        printf("lab8_1 pthread_create error: %s", strerror(r));
        printf("\n");
    }

    getchar();

    arg.flag = 1;

    r = pthread_join(th_func, (void **)&exitcode);
    if (r != 0)
    {
        printf("lab8_1 pthread_join error: %s", strerror(r));
        printf("\n");
    }
    else
    {
        printf("lab8_1 exitcode from thread_func: %d\n", exitcode);
    }

    mq_close(mq);
    if (r == -1)
    {
        perror("lab8_1 close error");
        return 1;
    }

    mq_unlink(mq_name);
    if (r == -1)
    {
        perror("lab8_1 unlink error");
        return 2;
    }
    
    return 0;
}