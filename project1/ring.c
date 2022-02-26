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
    pid_t parent_pid;
    int* *pipes;
    char number_of_processes[3];
    char destination_process[3];
    char message[MAX_MESSAGE_SIZE];
    int n_processes;

    struct apple {
        int destination;
        char message[MAX_MESSAGE_SIZE];
    };

    blue_fputs("How many processes should be created: ", stdout);
    // Get the number of processes from the user
    fgets(number_of_processes, 5, stdin);
    number_of_processes[strcspn(number_of_processes, "\n")] = 0;
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
    parent_pid = getpid();
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
    for(int i=2; i<=n_processes; ++i) {
        // Fork a child process and exit on error
        if((child_pids[i-2] = fork()) < 0) {
            fail("fork failed", 1);
        // Set up each child process
        } else if (!child_pids[i-2]) {
            //printf("Created node #%d (pid=%d)\n", i, getpid());
            // The first process should write to the second but read from the last
            int read_fd = i-2;
            int write_fd = i-1;
            struct apple the_apple;
            read(pipes[read_fd][READ], &the_apple, sizeof(the_apple));
            printf("Node #%d (pid=%d) recieved the apple...\n", i, getpid());
            sleep(1); //TODO uncomment this
            if(the_apple.destination != i) {
                if(the_apple.destination == 0) {
                    printf("Messaged already recieved. Sending apple home...\n");
                } else {
                    printf("Not the destination, passing it on...\n");
                }
                write(pipes[write_fd][WRITE], &the_apple, sizeof(the_apple));
            } else if(the_apple.destination == i) {
                printf("\033[0;32mReceived message: \033[0m%s\n", the_apple.message);
                the_apple.destination = 0;
                write(pipes[write_fd][WRITE], &the_apple, sizeof(the_apple));
            }

            if(i==n_processes) {
                kill(parent_pid, SIGINT);
            }

            exit(0); // Don't keep the loop running in the child process
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
    fgets(destination_process, 5, stdin);
    destination_process[strcspn(destination_process, "\n")] = 0;

    if(!(the_apple.destination = atoi(destination_process))) {
        fail("Destination process must be a valid number", 1);
    }
    if(the_apple.destination > n_processes || the_apple.destination < 1) {
        fail("Invalid destination process", 1);
    }

    blue_fputs("Message: ", stdout);
    fgets(message, MAX_MESSAGE_SIZE, stdin);
    message[strcspn(message, "\n")] = 0;
    strcpy(the_apple.message, message);

    printf("Node #1 (pid=%d) sending the apple off...\n", parent_pid);
    write(pipes[0][WRITE], &the_apple, sizeof(the_apple));
    struct apple new_apple;
    while(!time_to_die) {
        pause();
    }
    read(pipes[n_processes-1][READ], &new_apple, sizeof(the_apple));
    printf("Got the apple back. Message: %s\n", new_apple.message);

    // Free the memory and get out of here
    printf("Cleaning up...\n");
    free(child_pids);
    free(pipes);
    return 0;
}

// Gracefully handle ^C
void sig_handler(int sig_num, siginfo_t *info, void *ucontext) {
    if(sig_num == SIGINT) {
        time_to_die = true;
    }
}
