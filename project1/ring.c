#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>

#define WRITE 1
#define READ 0

bool time_to_die = false;

void sig_handler(int, siginfo_t*, void*);

int main() {
    pid_t *child_pids;
    int* *pipes;
    char input[32];
    int n_processes;

    printf("How many processes should be created: ");
    // Get the number of processes from the user
    fgets(input, 32, stdin);
    if(!(n_processes = atoi(input))) {
        perror("Invalid input\n");
        exit(1);
    }

    // Dynamically allocate arrays for pipes and pids based on input
    child_pids = malloc(sizeof(*child_pids) * n_processes);
    pipes = malloc(sizeof(*pipes) * n_processes);

    // Set up all the pipes
    for(int i=0; i<n_processes; ++i) {
        // Create new pipe and put it in the array
        int fd[2];
        if(pipe(fd) < 0) {
            perror("Failed to create pipe");
            exit(1);
        }
        pipes[i] = fd; 
    }

    // Create all the processes and set their stdout and stdin
    // to the appropriate pipes
    printf("Spawning %d processes\n", atoi(input));
    for(int i=0; i<n_processes; ++i) {
        // Fork a child process and exit on error
        if((child_pids[i] = fork()) < 0) {
            perror ("fork failed");
            exit(1);
        // Set up each child process
        } else if (!child_pids[i]) {
            printf("Created node #%d (pid=%d)\n", i, getpid());
            // The first process should write to the second but read from the last
            if(i==0) {
                printf("\tConnecting pipes...\n\t\tWRITE: %d\n\t\tREAD: %d\n", i+1, 0);
                dup2(pipes[i+1][WRITE], STDOUT_FILENO);
                dup2(pipes[0][READ], STDIN_FILENO);
            // The last process should write to the first and read from the second to last
            } else if(i==n_processes-1) {
                printf("\tConnecting pipes...\n\t\tWRITE: %d\n\t\tREAD: %d\n", 0, i);
                dup2(pipes[0][WRITE], STDOUT_FILENO);
                dup2(pipes[i][READ], STDIN_FILENO);
            // Each process should share a pipe with the next process and the previous
            } else {
                printf("\tConnecting pipes...\n\t\tWRITE: %d\n\t\tREAD: %d\n", i+1, i);
                dup2(pipes[i+1][WRITE], STDOUT_FILENO);
                dup2(pipes[i][READ], STDIN_FILENO);
            }
            break; // Don't keep the loop running in the child process
        // Parent process
        } else {
        }
    }
    printf("Hi I'm the parent(pid=%d)", getpid());
    printf("Done setting up child processes. Creating signal handler...\n");
    // Set up SIGINT handler
    struct sigaction new_action;
    new_action.sa_sigaction = &sig_handler;
    new_action.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &new_action, NULL);
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
