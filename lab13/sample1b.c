// Is this Sample Program 1?
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define SIZE 30
int main(int argc, char *argv[]){
    
    struct flock fileLock;   
    int fd;   
    char buf[SIZE] = "// Is this Sample Program 1?";   
    
    if (argc < 2) {      
        printf ("usage: filename\n");
        exit (1);
    }
        
    if ((fd = open (argv[1], O_RDWR)) < 0) {
        perror ("there is");
        exit (1);
    }   
        
    fileLock.l_type = F_WRLCK;    
    fileLock.l_whence = SEEK_SET;    
    fileLock.l_start = 0;    
    fileLock.l_len = 0;    

    // if (fcntl (fd, F_GETLK, &fileLock) < 0) {       
    //     perror ("Unable to obtain lock");       
    //     exit (1);    
    // }
    // else{
    //     printf("Lock acquired\n");
    // }

    //printf("%u\n", fcntl(fd, F_GETLK, &fileLock));

    while(fcntl(fd, F_SETLK, &fileLock) > 0){
        printf("Unable to aquire\n");
        sleep(1);
    }

    printf("Lock Aquired\n");

    //printf("Past Loop\n");  
    
    //write (fd, buf, SIZE-2);    
    //sleep (10);    
    close(fd);    
    return 0;
} 