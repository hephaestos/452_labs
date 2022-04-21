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

    while(fcntl(fd, F_SETLK, &fileLock) < 0){
        printf("Unable to aquire\n");
        sleep(1);
    }

    printf("Lock Aquired\n");

    
    char c[1000];

    FILE *fptr;

    if ((fptr = fopen(argv[1], "r")) == NULL) {

        printf("Error! File cannot be opened.");
        // Program exits if the file pointer returns NULL.
        exit(1);
    }

    // reads text until newline is encountered

    fscanf(fptr, "%[^\n]", c);

    printf("Data from the file:\n%s", c);

    fclose(fptr);


    close(fd);    
    return 0;
}
