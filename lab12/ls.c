#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

// Convert number of bytes to a string with appropriate units
void humanize(long bytes, char* humanized) {
    long out = bytes;
    char unit = 'B';

    if(bytes/1000 > 0) {
        out /= 1000;
        unit = 'K';
        if(out/1000 > 0) {
            out /= 1000;
            unit = 'M';
            if(out/1000 > 0) {
                out /= 1000;
                unit = 'G';
            }
        }
    }

    sprintf(humanized,"%ld%c", out, unit);
}

int main(int argc, char** argv) {
    bool nMode = false;
    bool iMode = false;
    char* filename;
    char* flags;

    // Handle args
    // Check # of args
    if(argc < 2 || argc > 3) {
        printf("Usage: ls [-ni] filename\n");
        exit(1);
    } else if(argc == 2) {
        filename = argv[1];
    } else if(argc == 3) {
        flags = argv[1];
        filename = argv[2];
        // Check that flags start with -
        if(*flags != '-') {
            printf("Invalid argument %s\n", flags);
            exit(1);
        }
        // Check through flags
        for(++flags; *flags != '\0'; flags++) {
            if(*flags == 'n') {
                nMode = true;
            } else if(*flags == 'i') {
                iMode = true;
            } else {
                printf("Invalid flag %c\n", *flags);
                exit(1);
            }
        }
    }

    DIR *dirPtr;
    struct dirent *entryPtr;
    if(!(dirPtr = opendir (filename))) {
        printf("%s is not a directory\n", filename);
        exit(2);
    }

    if(nMode && iMode) {
        printf("Filename            Size UID  GID  Inode\n");
        printf("----------------------------------------\n");
    } else if(nMode) {
        printf("Filename            Size UID  GID\n");
        printf("---------------------------------\n");
    } else if(iMode) {
        printf("Filename            Size Inode\n");
        printf("------------------------------\n");
    } else {
        printf("Filename            Size\n");
        printf("------------------------\n");
    }

    while ((entryPtr = readdir(dirPtr))) {
        struct stat fileStat;
        stat(entryPtr->d_name, &fileStat);
        char size[5];
        humanize(fileStat.st_size, size);
        
        if(nMode && iMode) {
            printf(
                "%-20s%-5s%d %d %ld\n",
                entryPtr->d_name,
                size,
                fileStat.st_uid,
                fileStat.st_gid,
                fileStat.st_ino
            );
        } else if(nMode) {
            printf(
                "%-20s%-5s%d %d\n",
                entryPtr->d_name,
                size,
                fileStat.st_uid,
                fileStat.st_gid
            );
        } else if(iMode) {
            printf(
                "%-20s%-5s%ld\n",
                entryPtr->d_name,
                size,
                fileStat.st_ino
            );
        } else {
            printf(
                "%-20s%-5s\n",
                entryPtr->d_name,
                size
            );
        }
    }

    closedir (dirPtr);
    return 0;
}
