#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define WRITE 1
#define READ 0

void sig_handler(int, siginfo_t*, void*);

int main() {
    pid_t *child_pids;
    int* *pipes;
    char input[32];
    int n_processes;

    printf("How many processes should be created: ");
    fgets(input, 32, stdin);
    if(!(n_processes = atoi(input))) {
        perror("Invalid input\n");
        exit(1);
    }

    child_pids = malloc(sizeof(*child_pids) * n_processes);
    pipes = malloc(sizeof(*pipes) * n_processes);
    printf("Spawning %d processes\n", atoi(input));
    for(int i=0; i<n_processes; ++i) {
        if ((child_pids[i] = fork()) < 0) {
            dup2(pipes[i+1][WRITE], STDOUT_FILENO);
            dup2(pipes[i][READ], STDIN_FILENO);
            perror ("fork failed");
            exit(1);
        } else if (!child_pids[i]) {
            printf("Created node #%d\n", i);
            break;
        }
    }


    struct sigaction new_action;
    new_action.sa_sigaction = &sig_handler;
    new_action.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &new_action, NULL);
}

void sig_handler(int sig_num, siginfo_t *info, void *ucontext) {
    if(sig_num == SIGINT) {
        printf(" received.  That's it, I'm shutting you down...\n");
        exit(0);
    }
}