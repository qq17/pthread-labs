#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 80

struct targs
{
    int flag;
};

mqd_t mq;
char mq_name[] = "/lab8_mq";

void * thread_func(void *arg)
{
    targs *args = (targs *) arg;
    char buffer[BUF_SIZE];
    int r;

    while(args->flag == 0)
    {
        memset(buffer, '\0', BUF_SIZE);
        r = mq_receive(mq, buffer, BUF_SIZE, 0);
        if (r > 0)
        {
            printf("received: %s\n", buffer);
        }
        else if (r == -1)
        {
            perror("lab8_2 mq_receive error");
            sleep(1);
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
    mq_attr attr;

    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = 15;
    attr.mq_msgsize = 60;
    attr.mq_curmsgs = 0;

    mq = mq_open(mq_name, O_CREAT | O_RDONLY, 0644, &attr);
    if (mq == (mqd_t)-1)
    {
        perror("lab8_2 mq_open error");
    }

    r = mq_getattr(mq, &attr);
    if (r == -1)
    {
        perror("lab8_2 mq_getattr error");
    }
    printf("flags: %ld\n", attr.mq_flags);
    printf("maxmsg: %ld\n", attr.mq_maxmsg);
    printf("msgsize: %ld\n", attr.mq_msgsize);
    printf("curmsgs: %ld\n", attr.mq_curmsgs);

    r = pthread_create(&th_func, NULL, thread_func, &arg);
    if (r != 0)
    {
        printf("lab8_2 pthread_create error: %s", strerror(r));
        printf("\n");
    }

    getchar();

    arg.flag = 1;

    r = pthread_join(th_func, (void **)&exitcode);
    if (r != 0)
    {
        printf("lab8_2 pthread_join error: %s", strerror(r));
        printf("\n");
    }
    else
    {
        printf("lab8_2 exitcode from thread_func: %d\n", exitcode);
    }

    mq_close(mq);
    if (r == -1)
    {
        perror("lab8_2 close error");
        return 1;
    }

    mq_unlink(mq_name);
    if (r == -1)
    {
        perror("lab8_2 unlink error");
        return 2;
    }
    
    return 0;
}