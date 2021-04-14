#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <limits.h>

int main()
{
    pid_t pid;
    int status;
    int r;

    printf("lab4_2 process id: %d\n", getpid());
    printf("lab4_2 parent process id: %d\n", getppid());
    printf("lab4_2 environment variable LANGUAGE: %s\n", getenv("LANGUAGE"));

    pid = fork();

    if (pid == 0)
    {
        char *path;
        char *newpath;
        char cwd[PATH_MAX];

        path = getenv("PATH");
        newpath = (char *)malloc(sizeof(char) * (strlen(path) + 1));
        strcpy(newpath, path);
        strcat(newpath, ":");
        strcat(newpath, getcwd(cwd, sizeof(cwd)));
        strcat(newpath, "/lab4_1:");
        setenv("PATH", newpath, 1);

        char *envp[] = {"LANGUAGE=fr", NULL};

        char *argv[] = {"lab4_1", "1", "5", "3", NULL};
        r = execvpe("lab4_1", argv, envp);
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
        printf("lab4_2 child process id: %d\n", pid);

        struct timespec t;
        t.tv_sec = 0;
        t.tv_nsec = 0.5 * 1000000000;

        while (r = waitpid(pid, &status, WNOHANG) == 0)
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