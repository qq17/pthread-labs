#include <iostream>
#include<pthread.h>
#include<string>
#include <sys/types.h>
#include <sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include <fcntl.h> 
#include <ctime>
#include <signal.h>
using namespace std;
#define  SIZE 200

char fifoName[] = {"/tmp/my_named_pipe"};
int fd;
int flag = 0;
pthread_t id_thread_open;
pthread_t id_thread_exec;
void * write_pipe(void * arg){
     cout <<"\nStart write with thread id: " << pthread_self() << endl;  
     char buffer[SIZE]; 
     time_t rawtime;
    tm *loctime;
     while(flag != 1){
         rawtime = time(0);
        loctime = localtime(&rawtime);
        int r = strftime(buffer, SIZE, "%Y.%m.%d %H:%M:%S", loctime);
         printf("\n Write: %s",buffer); 
         ssize_t re_value = write(fd,buffer,r);
         if(re_value == -1){
             cout <<"\nFailed  write file with error: " << strerror(errno);
             sleep(1);
             continue;
         }
         sleep(1);
     }
     return NULL;
}
void* open_fifo(void* args){
    while (flag != 1){
        fd = open(fifoName, O_WRONLY | O_NONBLOCK);
        if (fd == -1){
            perror("\nFailed to open file: ");
            sleep(1);
        }
        else {
            pthread_create(&id_thread_exec , NULL , write_pipe, NULL);
            return NULL;
        }   
    }
    return NULL;
}


void sig_handler(int signo){
    cout << "\nSIGPIPE is gotten!";
}
int main()
{
    printf("\nThe program started working");
    signal(SIGPIPE, sig_handler);
    mkfifo(fifoName, 0644);
    if(pthread_create(&id_thread_open,NULL,open_fifo,NULL) != 0){
    cout <<"\nError function pthread_create for 1-th thread: " << strerror(errno);
    sleep(3);
    exit(EXIT_FAILURE);
    }
    printf("\nThe program waits for a key press");
    getchar();
    printf("\nKey pressed\n");
    flag =1;
    if(pthread_join(id_thread_open, NULL) != 0){
    cout <<"\nError function pthread_join for 1-th thread: " << strerror(errno);
    sleep(3);
    exit(EXIT_FAILURE);
    } 
    if(pthread_join(id_thread_exec, NULL) != 0){
    cout <<"\nError function pthread_join for 1-th thread: " << strerror(errno);
    sleep(3);
    exit(EXIT_FAILURE);
    } 
    close(fd);
    unlink(fifoName);
    printf("\nProgram terminated\n");
    return 0;
}

