#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

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

int main() {
    DIR *dirPtr;
    struct dirent *entryPtr;

    dirPtr = opendir (".");


    while ((entryPtr = readdir (dirPtr))) {
        struct stat fileStat;
        stat(entryPtr->d_name, &fileStat);
        printf ("%ld: %-20s\n", fileStat.st_size, entryPtr->d_name);
    }

    closedir (dirPtr);
    return 0;
}
