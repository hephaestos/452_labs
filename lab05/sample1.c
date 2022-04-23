#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define FOO 4096

int main () {
   int shmId;
   char *shmPtr;
   struct shmid_ds info;

   if ((shmId = shmget (IPC_PRIVATE, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
      perror ("i can't get no..\n");
      exit (1);
   }
   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      perror ("can't attach\n");
      exit (1);
   }
   printf ("value a: %p\t value b: %p\n", (void *) shmPtr, (void *) shmPtr + FOO);
   if (shmdt (shmPtr) < 0) {
      perror ("just can't let go\n");
      exit (1);
   }
   if ( shmctl(shmId, IPC_STAT, &info) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }

   printf("Shared Memory Size: [%ld]\n", info.shm_segsz);

   return 0;
}
