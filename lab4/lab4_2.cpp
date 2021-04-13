#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
    pid_t pid;
    int status;
    int r;

    pid = fork();

    if (pid == 0)
    {
        char *argv[] = {"./lab4_1", "1", "5", "3", NULL};
        r = execvpe("./lab4_1", argv, NULL);
        if (r == -1)
        {
            perror("execvpe error\n");
        }
    }
    else if (pid == -1)
    {
        perror("fork error\n");
    }
    else
    {
        printf("lab4_2 process id: %d\n", getpid());
        printf("lab4_2 parent process id: %d\n", getppid());
        printf("lab4_2 child process id: %d\n", pid);

        struct timespec t;
        t.tv_sec = 0;
        t.tv_nsec = 0.5 * 1000000000;

        while (waitpid(pid, &status, WNOHANG) == 0)
        {
            printf("Waiting for child process termination\n");
            nanosleep(&t, NULL);
        }
        
        if (WIFEXITED(status))
        {
            printf("exitcode from child process: %d\n", WEXITSTATUS(status));
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
    
    return 0;
}