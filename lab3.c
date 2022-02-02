#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sig_handler(int);

int main()
{
    pid_t child_pid;
    pid_t parent_pid = getpid();

    if ((child_pid = fork()) < 0) {
        perror ("fork failed");
        exit(1);
    } else if (!child_pid) {
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
        printf("spawned child PID# %d\n", child_pid);
        write(STDOUT_FILENO, "waiting...\t", 12);
        signal(SIGINT, sig_handler);
        signal(SIGUSR1, sig_handler);
        signal(SIGUSR2, sig_handler);
        while(1) {
            pause();
        }
    }
}

void sig_handler(int sig_num) {
    if(sig_num == SIGINT) {
        write(STDOUT_FILENO, " received.  That's it, I'm shutting you down...\n", 49);
        exit(0);
    }
    if (sig_num == SIGUSR1) {
        write(STDOUT_FILENO, "received a SIGUSR1 signal\n", 27);
        write(STDOUT_FILENO, "waiting...\t", 12);
        return;
    }
    if (sig_num == SIGUSR2) {
        write(STDOUT_FILENO, "received a SIGUSR2 signal\n", 27);
        write(STDOUT_FILENO, "waiting...\t", 12);
        return;
    }
}