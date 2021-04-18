#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define SHM_SIZE 80

struct targs
{
    int flag;
};

sem_t *sem_write;
sem_t *sem_read;
int shm;
int *addr;

void * thread_func(void *arg)
{
    targs *args = (targs *) arg;
    char buffer[SHM_SIZE];
    int r;

    while(args->flag == 0)
    {
        r = sem_wait(sem_write);
        if (r == -1)
        {
            perror("lab6_2 sem_wait error");
            pthread_exit((void *)15);
        }

        memcpy(buffer, addr, SHM_SIZE);
        printf("%s\n", buffer);

        r = sem_post(sem_read);
        if (r == -1)
        {
            perror("lab6_2 sem_post error");
            pthread_exit((void *)16);
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
    pthread_t t;

    shm = shm_open("/lab6_shared_memory", O_CREAT | O_RDWR, 0644);
    if (r == -1)
    {
        perror("lab6_2 shm_open error");
        return 1;
    }

    ftruncate(shm, SHM_SIZE);
    if (r == -1)
    {
        perror("lab6_2 ftruncate error");
        return 2;
    }

    addr = (int *)mmap(NULL, SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);
    if (addr == MAP_FAILED)
    {
        perror("lab6_2 mmap error");
        return 3;
    }

    sem_write = sem_open("/lab6_semaphore_write", O_CREAT, 0644, 0);
    if (sem_write == SEM_FAILED)
    {
        perror("lab6_2 sem_open error");
        return 4;
    }

    sem_read = sem_open("/lab6_semaphore_read", O_CREAT, 0644, 0);
    if (sem_read == SEM_FAILED)
    {
        perror("lab6_2 sem_open error");
        return 5;
    }

    r = pthread_create(&t, NULL, thread_func, &arg);
    if (r != 0)
    {
        printf("lab6_2 pthread_create error: %s", strerror(r));
        printf("\n");
        return 6;
    }

    getchar();

    arg.flag = 1;

    r = pthread_join(t, (void **)&exitcode);
    if (r != 0)
    {
        printf("lab6_2 pthread_join error: %s", strerror(r));
        printf("\n");
        return 7;
    }

    printf("lab6_2 exitcode from thread_func: %d\n", exitcode);

    r = sem_close(sem_write);
    if (r == -1)
    {
        perror("lab6_2 sem_close error");
        return 8;
    }

    r = sem_unlink("/lab6_semaphore_write");
    if (r == -1)
    {
        perror("lab6_2 sem_unlink error");
        return 9;
    }

    r = sem_close(sem_read);
    if (r == -1)
    {
        perror("lab6_2 sem_close error");
        return 10;
    }

    r = sem_unlink("/lab6_semaphore_read");
    if (r == -1)
    {
        perror("lab6_2 sem_unlink error");
        return 11;
    }

    r = munmap(addr, sizeof(char));
    if (r == -1)
    {
        perror("lab6_2 munmap error");
        return 12;
    }

    close(shm);
    if (r == -1)
    {
        perror("lab6_2 close error");
        return 13;
    }

    shm_unlink("/lab6_shared_memory");
    if (r == -1)
    {
        perror("lab6_2 shm_unlink error");
        return 14;
    }
    
    return 0;
}