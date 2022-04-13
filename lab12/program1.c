#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  struct stat statBuf;

  if (argc < 2) {
    printf ("Usage: argument (filename) expected\n");
    exit(1);
  }

  if (stat (argv[1], &statBuf) < 0) {
    perror ("Program error: ");
    exit(1);
  }

  mode_t current_mode = statBuf.st_mode;

  if (S_ISDIR(current_mode)) {
    printf("%s is a directory\n", argv[1]);
  }
  else {
    printf("%s is NOT a directory\n", argv[1]);
  }

  printf("value is: %o\n", current_mode);
  printf("inode value is: %lu\n", statBuf.st_ino);
  
  return 0;
}
