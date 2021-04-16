#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

struct targs
{
    int flag;
};

sem_t *sem;
FILE *fd;

void * thread_func(void *arg)
{
    targs *args = (targs *) arg;
    char c = '2';
    int r;

    while(args->flag == 0)
    {
        r = sem_wait(sem);
        if (r == -1)
        {
            perror("lab5_2 sem_wait error");
            pthread_exit((void *)8);
        }

        for (int i=0; i<3; ++i)
        {
            fprintf(fd, "%c", c);
            putchar(c);
            fflush(stdout);
            fflush(fd);
            sleep(1);
        }

        r = sem_post(sem);
        if (r == -1)
        {
            perror("lab5_2 sem_post error");
            pthread_exit((void *)9);
        }

        sleep(1);
    }
    pthread_exit((void *)0);
}

int main()
{
    targs arg;
    arg.flag = 0;
    int r;
    int exitcode;
    pthread_t t;

    sem = sem_open("/semaphore", O_CREAT, 0644, 1);
    if (sem == SEM_FAILED)
    {
        perror("lab5_2 sem_open error");
        return 1;
    }

    fd = fopen("lab5_file.txt", "a+");
    if (fd == NULL)
    {
        perror("lab5_2 fopen error");
        return 2;
    }

    r = pthread_create(&t, NULL, thread_func, &arg);
    if (r != 0)
    {
        printf("lab5_2 pthread_create error: %s", strerror(r));
        printf("\n");
        return 3;
    }

    getchar();

    arg.flag = 1;

    r = pthread_join(t, (void **)&exitcode);
    if (r != 0)
    {
        printf("lab5_2 pthread_join error: %s", strerror(r));
        printf("\n");
        return 4;
    }

    printf("lab5_2 exitcode from thread_func: %d\n", exitcode);

    r = fclose(fd);
    if (r == EOF)
    {
        perror("lab5_2 fclose error");
        return 5;
    }

    r = sem_close(sem);
    if (r == -1)
    {
        perror("lab5_2 sem_close error");
        return 6;
    }

    r = sem_unlink("/semaphore");
    if (r == -1)
    {
        perror("lab5_2 sem_unlink error");
        return 7;
    }
    
    return 0;
}