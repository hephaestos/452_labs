#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>

#define WRITE 1
#define READ 0
#define MAX_MESSAGE_SIZE 999

bool time_to_die = false;

void sig_handler(int, siginfo_t*, void*);

void blue_fputs(char* s, FILE* stream) {
    fputs("\033[0;34m", stream);
    fputs(s, stream);
    fputs("\033[0m", stream);
}

void fail(char* error, int status) {
    puts("\033[0;31m");
    perror(error);
    puts("\033[0m");
    exit(status);
}

int main() {
    pid_t *child_pids;
    int* *pipes;
    char number_of_processes[3];
    char destination_process[3];
    char message[MAX_MESSAGE_SIZE];
    int n_processes;

    struct apple {
        int destination;
        char *message;
    };

    blue_fputs("How many processes should be created: ", stdout);
    // Get the number of processes from the user
    fgets(number_of_processes, 3, stdin);
    if(!(n_processes = atoi(number_of_processes))) {
        fail("Invalid input", 1);
    }
    if(n_processes < 0 || n_processes > 999) {
        fail("Please enter a number 1-999", 1);
    }

    // Dynamically allocate arrays for pipes and pids based on input
    child_pids = malloc(sizeof(*child_pids) * n_processes);
    pipes = malloc(sizeof(*pipes) * n_processes);

    // Set up all the pipes
    printf("Creating %d pipes...\n", n_processes);
    for(int i=0; i<n_processes; ++i) {
        // Create new pipe and put it in the array
        int fd[2];
        if(pipe(fd) < 0) {
            fail("Failed to create pipe", 1);
        }
        pipes[i] = fd; 
    }

    // Create all the processes and set their stdout and stdin
    // to the appropriate pipes
    printf("Forking %d processes...\n", n_processes);
    for(int i=0; i<n_processes; ++i) {
        // Fork a child process and exit on error
        if((child_pids[i] = fork()) < 0) {
            fail("fork failed", 1);
        // Set up each child process
        } else if (!child_pids[i]) {
            //printf("Created node #%d (pid=%d)\n", i, getpid());
            // The first process should write to the second but read from the last
            int read_fd, write_fd;
            if(i==0) {
                //printf("Node %d: \n\tRead from pipe: %d\n\tWrite to pipe: %d\n", i, 0, i+1);
                read_fd = 0;
                write_fd = i+1;
            // The last process should write to the first and read from the second to last
            } else if(i==n_processes-1) {
                //printf("Node %d: \n\tRead from pipe: %d\n\tWrite to pipe: %d\n", i, i, 0);
                read_fd = i;
                write_fd = 0;
            // Each process should share a pipe with the next process and the previous
            } else {
                //printf("Node %d: \n\tRead from pipe: %d\n\tWrite to pipe: %d\n", i, i, i+1);
                read_fd = i;
                write_fd = i+1;
            }
            exit(0); // Don't keep the loop running in the child process
        // Parent process
        } else {
        }
    }
    puts("Done setting up child processes. Creating signal handler...");
    // Set up SIGINT handler
    struct sigaction new_action;
    new_action.sa_sigaction = &sig_handler;
    new_action.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &new_action, NULL);
    
    struct apple the_apple;

    blue_fputs("Destination node: ", stdout);
    fgets(destination_process, 3, stdin);

    if(!(the_apple.destination = atoi(destination_process))) {
        fail("Destination process must be a valid number", 1);
    }
    if(the_apple.destination > n_processes) {
        fail("Invalid destination process", 1);
    }

    blue_fputs("Message: ", stdout);
    fgets(message, MAX_MESSAGE_SIZE, stdin);
    the_apple.message = malloc(MAX_MESSAGE_SIZE * sizeof(*the_apple.message));
    strcpy(the_apple.message, message);

    fputs("Sending message to node ", stdout);
    fputs(destination_process, stdout);
    fputs("Message: ", stdout);
    fputs(the_apple.message, stdout);

    while(!time_to_die) {
        pause();
    }

    // Free the memory and get out of here
    printf("Cleaning up...\n");
    free(child_pids);
    free(pipes);
    return 0;
}

// Gracefully handle ^C
void sig_handler(int sig_num, siginfo_t *info, void *ucontext) {
    if(sig_num == SIGINT) {
        printf(" received.  That's it, I'm shutting you down...\n");
        time_to_die = true;
    }
}
