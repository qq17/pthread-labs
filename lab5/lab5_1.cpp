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
    char c = '1';
    int r;

    while(args->flag == 0)
    {
        sem_wait(sem);
    	fprintf(fd, "%c", c);
        putchar(c);
        fflush(stdout);
        fflush(fd);
        sem_post(sem);
        sleep(1);
    }
    
}

int main()
{
    targs arg;
    arg.flag = 0;
    int r;
    pthread_t t;

    sem = sem_open("/semaphore", O_CREAT, 0644, 1);

    fd = fopen("lab5_file.txt", "a+");
    if (fd == NULL)
    {
    	perror("lab5_2 fopen error");
    }

    r = pthread_create(&t, NULL, thread_func, &arg);
    if (r != 0)
    {
        printf("lab5_2 pthread_create error: %s", strerror(r));
        printf("\n");
    }

    getchar();

    arg.flag = 1;

    r = pthread_join(t, NULL);
    if (r != 0)
    {
        printf("lab5_2 pthread_join error: %s", strerror(r));
        printf("\n");
    }

    r = fclose(fd);
    if (r == EOF)
    {
    	perror("lab5_2 fclose error");
    }

    sem_close(sem);

    sem_unlink("/semaphore");
    
    return 0;
}