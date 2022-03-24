#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

int main() {
    char *data1;
    size_t len = 16;
    ssize_t read;

    data1 = malloc(SIZE);
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
