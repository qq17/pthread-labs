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
    char c[SHM_SIZE];
    int r;

    r = sem_wait(sem_write);

    while(args->flag == 0)
    {
        for (int i=0; i<3; ++i)
        {
            memcpy(c, addr, SHM_SIZE);
            printf("%s\n", c);
            fflush(stdout);
            sleep(1);
        }

        r = sem_post(sem_read);
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
    ftruncate(shm, SHM_SIZE);
    addr = (int *)mmap(NULL, SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);

    sem_write = sem_open("/lab6_semaphore_write", O_CREAT, 0644, 0);
    sem_read = sem_open("/lab6_semaphore_read", O_CREAT, 0644, 0);

    r = pthread_create(&t, NULL, thread_func, &arg);
    if (r != 0)
    {
        printf("lab6_2 pthread_create error: %s", strerror(r));
        printf("\n");
        return 3;
    }

    getchar();

    arg.flag = 1;

    r = pthread_join(t, (void **)&exitcode);
    if (r != 0)
    {
        printf("lab6_2 pthread_join error: %s", strerror(r));
        printf("\n");
        return 4;
    }

    printf("lab6_2 exitcode from thread_func: %d\n", exitcode);


    r = sem_close(sem_write);

    r = sem_unlink("/lab6_semaphore_write");

    r = sem_close(sem_read);

    r = sem_unlink("/lab6_semaphore_read");

    munmap(addr, sizeof(char));
    close(shm);

    shm_unlink("/lab6_shared_memory");
    
    return 0;
}