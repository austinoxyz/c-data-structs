#include <stdio.h>
#include <stdlib.h>
int main(void) {

    int *buffer = (int *) malloc(10 * sizeof(int));
    if (!buffer) {
        printf("failed allocation!\n");
        exit(1);
    }

    buffer = (int *) realloc(buffer, 10000);
    if (!buffer) {
        printf("failed allocation!\n");
        exit(1);
    }

    for (int i = 0; i < 10000; ++i)
        buffer[i] = i;

    printf("Hello, World!\n");
}
