#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define handle_error_en(en, msg) \
       do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

struct targs
{
    int flag;
};

static void
display_pthread_attr(pthread_attr_t *attr, char *prefix)
{
    int s, i;
    size_t v;
    void *stkaddr;
    struct sched_param sp;

    s = pthread_attr_getdetachstate(attr, &i);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getdetachstate");
    printf("%sDetach state        = %s\n", prefix,
            (i == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
            (i == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" :
            "???");

    s = pthread_attr_getscope(attr, &i);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getscope");
    printf("%sScope               = %s\n", prefix,
            (i == PTHREAD_SCOPE_SYSTEM)  ? "PTHREAD_SCOPE_SYSTEM" :
            (i == PTHREAD_SCOPE_PROCESS) ? "PTHREAD_SCOPE_PROCESS" :
            "???");

    s = pthread_attr_getinheritsched(attr, &i);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getinheritsched");
    printf("%sInherit scheduler   = %s\n", prefix,
            (i == PTHREAD_INHERIT_SCHED)  ? "PTHREAD_INHERIT_SCHED" :
            (i == PTHREAD_EXPLICIT_SCHED) ? "PTHREAD_EXPLICIT_SCHED" :
            "???");

    s = pthread_attr_getschedpolicy(attr, &i);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getschedpolicy");
    printf("%sScheduling policy   = %s\n", prefix,
            (i == SCHED_OTHER) ? "SCHED_OTHER" :
            (i == SCHED_FIFO)  ? "SCHED_FIFO" :
            (i == SCHED_RR)    ? "SCHED_RR" :
            "???");

    s = pthread_attr_getschedparam(attr, &sp);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getschedparam");
    printf("%sScheduling priority = %d\n", prefix, sp.sched_priority);

    s = pthread_attr_getguardsize(attr, &v);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getguardsize");
    printf("%sGuard size          = %zu bytes\n", prefix, v);

    s = pthread_attr_getstack(attr, &stkaddr, &v);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getstack");
    printf("%sStack address       = %p\n", prefix, stkaddr);
    printf("%sStack size          = %#zx bytes\n", prefix, v);
}

void * f1(void *arg)
{
    targs *args = (targs *) arg;
    int r;
    pthread_attr_t gattr;

    r = pthread_getattr_np(pthread_self(), &gattr);
    if (r != 0)
        handle_error_en(r, "pthread_getattr_np");

    printf("Thread attributes:\n");
    display_pthread_attr(&gattr, "\t");

    while(args->flag == 0)
    {
        putchar('1');
        fflush(stdout);
        sleep(1);
    }
    pthread_exit((void *)55);
}

void * f2(void *arg)
{
    targs *args = (targs *) arg;
    int r;
    pthread_attr_t gattr;

    r = pthread_getattr_np(pthread_self(), &gattr);
    if (r != 0)
        handle_error_en(r, "pthread_getattr_np");

    printf("Thread attributes:\n");
    display_pthread_attr(&gattr, "\t");

    while(args->flag == 0)
    {
        putchar('2');
        fflush(stdout);
        sleep(1);
    }
    pthread_exit((void *)77);
}

int main()
{
    targs arg1;
    targs arg2;
    arg1.flag = 0;
    arg2.flag = 0;
    int r1;
    int r2;
    int r;
    pthread_t t1;
    pthread_t t2;

    pthread_attr_t attr1;
    pthread_attr_t attr2;

    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);

    r = pthread_create(&t1, &attr1, f1, &arg1);
    if (r != 0)
    {
        printf("pthread_create error: %s", strerror(r));
    }
    r = pthread_create(&t2, &attr2, f2, &arg2);
    if (r != 0)
    {
        printf("pthread_create error: %s", strerror(r));
    }

    getchar();

    arg1.flag = 1;
    arg2.flag = 1;

    r = pthread_join(t1, (void **)&r1);
    if (r != 0)
    {
        printf("pthread_join error: %s", strerror(r));
    }
    r = pthread_join(t2, (void **)&r2);
    if (r != 0)
    {
        printf("pthread_join error: %s", strerror(r));
    }

    printf("exitcode from t1 = %d\n", r1);
    printf("exitcode from t2 = %d\n", r2);

    pthread_attr_destroy(&attr1);
    pthread_attr_destroy(&attr2);
    
    return 0;
}

/*
Thread attributes:
    Detach state        = PTHREAD_CREATE_JOINABLE
    Scope               = PTHREAD_SCOPE_SYSTEM
    Inherit scheduler   = PTHREAD_INHERIT_SCHED
    Scheduling policy   = SCHED_OTHER
    Scheduling priority = 0
    Guard size          = 4096 bytes
    Stack address       = 0x7fad75980000
    Stack size          = 0x800000 bytes
2Thread attributes:
    Detach state        = PTHREAD_CREATE_JOINABLE
    Scope               = PTHREAD_SCOPE_SYSTEM
    Inherit scheduler   = PTHREAD_INHERIT_SCHED
    Scheduling policy   = SCHED_OTHER
    Scheduling priority = 0
    Guard size          = 4096 bytes
    Stack address       = 0x7fad76181000
    Stack size          = 0x800000 bytes
1212121
exitcode from t1 = 55
exitcode from t2 = 77
*/