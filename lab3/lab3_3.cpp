#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctime>
#include <fcntl.h>

struct targs
{
    int flag;
};

int pipefd[2];

void * writer(void *arg)
{
    targs *args = (targs *) arg;
    int r;

    char buffer[20];
    time_t rawtime;
    tm *loctime;
    
    while(args->flag == 0)
    {
        rawtime = time(0);
        loctime = localtime(&rawtime);
        strftime(buffer, 20, "%Y.%m.%d %H:%M:%S", loctime);

        r = write(pipefd[1], buffer, 20);
        if (r == -1)
        {
            perror("write error: ");
        }

        sleep(1);
    }
}

void * reader(void *arg)
{
    targs *args = (targs *) arg;
    int r;
    char buffer[20];
    
    while(args->flag == 0)
    {
        memset(buffer, '\0', 20);
        
        r = read(pipefd[0], buffer, 20);
        if (r == -1)
        {
            perror("read error: ");
        }
        printf("%s\n", buffer);
        
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

    r = pipe(pipefd);
    if (r == -1)
    {
        perror("pipe error: ");
    }

    r = fcntl(pipefd[0], F_SETFL, O_NONBLOCK);
    if (r == -1)
    {
        perror("fcntl error: ");
    }

    r = pthread_create(&t1, NULL, writer, &arg1);
    if (r != 0)
    {
        printf("t1 pthread_create error: %s", strerror(r));
        printf("\n");
    }
    r = pthread_create(&t2, NULL, reader, &arg2);
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
        printf("t1 pthread_join error: %s", strerror(r));
        printf("\n");
    }
    r = pthread_join(t2, NULL);
    if (r != 0)
    {
        printf("t2 pthread_join error: %s", strerror(r));
        printf("\n");
    }

    r = close(pipefd[1]);
    if (r == -1)
    {
        perror("pipefd[1] close error: ");
    }
    r = close(pipefd[0]);
    if (r == -1)
    {
        perror("pipefd[0] close error: ");
    }
    
    return 0;
}