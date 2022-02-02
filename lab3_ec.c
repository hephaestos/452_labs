#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sig_handler(int, siginfo_t*, void*);

int main()
{
    pid_t child_a_pid;
    pid_t child_b_pid;
    pid_t parent_pid = getpid();

    if ((child_a_pid = fork()) < 0) {
        perror ("fork failed");
        exit(1);
    } else if (!child_a_pid) {
        srand(child_a_pid);
        while(1) {
            int time_to_sleep = rand()%5 + 1;
            sleep(time_to_sleep);
            if(rand()%2 == 0) {
                kill(parent_pid, SIGUSR1);
            } else {
                kill(parent_pid, SIGUSR2);
            }
        }
    } else {
        if ((child_b_pid = fork()) < 0) {
            perror ("fork failed");
            exit(1);
        } else if (!child_b_pid) {
            srand(child_b_pid);
            while(1) {
                int time_to_sleep = rand()%5 + 1;
                sleep(time_to_sleep);
                if(rand()%2 == 0) {
                    kill(parent_pid, SIGUSR1);
                } else {
                    kill(parent_pid, SIGUSR2);
                }
            }
        } else {
            printf("spawned child A PID# %d and child B PID# %d\n", child_a_pid, child_b_pid);
            printf("waiting...\t");

            struct sigaction new_action;
            new_action.sa_sigaction = &sig_handler;
            new_action.sa_flags = SA_SIGINFO;
            sigaction(SIGINT, &new_action, NULL);
            sigaction(SIGUSR1, &new_action, NULL);
            sigaction(SIGUSR2, &new_action, NULL);

            while(1) {
                pause();
            }
        }
    }
}

void sig_handler(int sig_num, siginfo_t *info, void *ucontext) {
    if(sig_num == SIGINT) {
        printf(" received.  That's it, I'm shutting you down...\n");
        exit(0);
    }
    if (sig_num == SIGUSR1) {
        printf("received a SIGUSR1 signal from PID:%d\n", info->si_pid);
        printf("waiting...\t");
        return;
    }
    if (sig_num == SIGUSR2) {
        printf("received a SIGUSR2 signal from PID:%d\n", info->si_pid);
        printf("waiting...\t");
        return;
    }
}