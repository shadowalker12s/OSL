#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n;
    printf("Enter the number of elements in the array: ");
    scanf("%d", &n);
    int arr[n];
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    int child_pid = fork();
    if (child_pid == 0) { // Child process
        printf("Main process start: (pid) %d\n", getpid());

        // Convert sorted array to command-line arguments
        char* sorted_arr[n + 2];
        sorted_arr[0] = "./2bchild";
        for (int i = 0; i < n; i++) {
            sorted_arr[i + 1] = (char*)malloc(10);
            sprintf(sorted_arr[i + 1], "%d", arr[i]);
        }
        sorted_arr[n + 1] = NULL;

        execve(sorted_arr[0], sorted_arr, NULL);
        perror("execve failed"); // Only prints if execve fails
    } else if (child_pid > 0) { // Parent process
        wait(NULL); // Wait for child to complete
        bubbleSort(arr, n);
        
        printf("Sorted array: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    } else {
        perror("fork failed");
        exit(1);
    }

    return 0;
}
