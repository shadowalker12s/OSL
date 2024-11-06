#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    printf("Child process start: (pid) %d\n", getpid());
    printf("Reversed sorted array: ");
    for (int i = argc - 1; i > 0; i--) { // Display in reverse order
        printf("%s ", argv[i]);
    }
    printf("\n");
    printf("Child process end: (pid) %d\n", getpid());
    return 0;
}
