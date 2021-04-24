#include <iostream>
#include<pthread.h>
#include<string>
#include<pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include <fcntl.h> 
#include <signal.h>
using namespace std;
#define  SIZE 200

char fifoName[] = {"/tmp/my_named_pipe"};
int fd;
int flag = 0;

void * read_pipe(void * arg){
     cout <<"\nStart read with thread id: " << pthread_self() << endl;  
     char buffer[SIZE]; 
     while(flag != 1){
        memset(buffer,0,SIZE);
        ssize_t re_value = read(fd,buffer,SIZE);
         if(re_value == -1)
         {
         cout <<"\nFailed read with error: " << strerror(errno);
         sleep(1);
         }
         else if(re_value == 0){
         cout <<"\nEnd of file, pipe for writing is closed";
         sleep(1);
         }
         else
             cout <<"\nRead: " << buffer;
     }

     return NULL;
}

int main()
{
    printf("\nThe program started working");
    pthread_t id1;
    mkfifo(fifoName, 0644);
    fd = open(fifoName,O_RDONLY|O_NONBLOCK); 
    if (fd == -1){
        perror("Failed to open file: ");
        sleep(1);
    }
    if(pthread_create(&id1,NULL,read_pipe,NULL) != 0){
    cout <<"\nError function pthread_create for 1-th thread: " << strerror(errno);
    sleep(3);
    exit(EXIT_FAILURE);
    }
    printf("\nThe program waits for a key press");
    getchar();
    printf("\nKey pressed\n");
    flag =1;
    if(pthread_join(id1, NULL) != 0){
    cout <<"\nError function pthread_join for 1-th thread: " << strerror(errno);
    sleep(3);
    exit(EXIT_FAILURE);
    } 
    close(fd);
    unlink(fifoName);
    printf("\nProgram terminated\n");
    return 0;
}

