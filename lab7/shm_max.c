#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main() {
    int shmId = 0;
    int keyId = 70;
    key_t shmKey = ftok("shmtext", keyId);
    unsigned long long size;
    printf("%ld\n", sysconf(_SC_PAGESIZE));
    
    for(size=17636708300ull; shmId >= 0; size++) {
        printf("Trying size %llu\n", size);
        shmId = shmget(shmKey, size, 0666|IPC_CREAT);
        shmctl(shmId, IPC_RMID, 0);
    }
    printf("Couldn't allocate segment of size %llu\nMax segment size must be: %llu\n", size-1, size-2);
}
