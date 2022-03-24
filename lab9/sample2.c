#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int size = 16;
char *data1;

int main() {
    
    size_t len = 0;
    ssize_t read;

    data1 = malloc(size);
    printf("Please input username: ");
    if((read = getline(&data1, &len, stdin)) != -1) {
        printf("you entered: %s\n", data1);
    }
    if(ferror(stdin)) {
        perror("Failed to read input\n");
    }
    free(data1);
    return 0;
}