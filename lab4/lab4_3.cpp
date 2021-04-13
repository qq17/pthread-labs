#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <time.h>
#include <limits.h>

#define STACK_SIZE (1024 * 1024)

static int childFunc(void *arg)
{
    char *path;
    char cwd[PATH_MAX];
    int r;

    path = getenv("PATH");
    strcat(path, ":");
    strcat(path, getcwd(cwd, sizeof(cwd)));
    strcat(path, "/lab4_1:");
    setenv("PATH", path, 1);

    char *argv[] = {"lab4_1", "1", "5", "3", NULL};
    r = execvpe("lab4_1", argv, NULL);
    if (r == -1)
    {
        perror("execvpe error\n");
    }
}

int main()
{
    char *stack;
    char *stackTop;
    int childPid;
    int status;
    int r;

    printf("lab4_3 process id: %d\n", getpid());
    printf("lab4_3 parent process id: %d\n", getppid());

    stack = (char *)malloc(STACK_SIZE);
    stackTop = stack + STACK_SIZE;

    childPid = clone(childFunc, stackTop, SIGCHLD, NULL);

    if (childPid == -1)
    {
        perror("clone error\n");
    }
    else
    {
        printf("lab4_3 child process id: %d\n", childPid);

        struct timespec t;
        t.tv_sec = 0;
        t.tv_nsec = 0.5 * 1000000000;

        while (r = waitpid(childPid, &status, WNOHANG) == 0)
        {
            printf("Waiting for child process termination\n");
            nanosleep(&t, NULL);
        }
        
        if (r == -1)
        {
            perror("waitpid error");
        }
        else
        {
            if (WIFEXITED(status))
            {
                printf("exitstatus from child process: %d\n", WEXITSTATUS(status));
            }
            else if (WIFSIGNALED(status))
            {
                printf("child process terminated by signal: %d\n", WTERMSIG(status));
            }
            else if (WIFSTOPPED(status))
            {
                printf("child process stopped by signal: %d\n", WSTOPSIG(status));
            }
        }
    }
    
    return 0;
}