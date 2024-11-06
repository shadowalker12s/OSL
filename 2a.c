#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void bubble_sort(int arr[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void show(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d\t", arr[i]);
    }
    printf("\n");
}

int main() {
    pid_t pid;
    int n;

    printf("Enter the size of array: ");
    scanf("%d", &n);
    int arr[n];

    printf("Enter the elements:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    int choice;
    printf("1. Normal Process\n");
    printf("2. Orphan Process\n");
    printf("3. Zombie Process\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    printf("Fork program starting\n");
    pid = fork(); // Creating child process

    switch (choice) {
        case 1: // Normal Process
            if (pid < 0) {
                perror("fork failed");
                exit(1);
            } else if (pid == 0) { // Child process
                printf("\nChild process (normal) is %d\n", getpid());
                bubble_sort(arr, n);
                printf("\nThe result of sorting by child process is: \n");
                show(arr, n);
            } else { // Parent process
                wait(NULL);
                printf("Parent process (normal) is %d\n", getpid());
                mergeSort(arr, 0, n - 1);
                printf("The result of sorting by parent process is: \n");
                show(arr, n);
            }
            break;

        case 2: // Orphan Process
            if (pid < 0) {
                perror("fork failed");
                exit(1);
            } else if (pid == 0) { // Child process
                printf("\nBEFORE Child process (orphan) is %d\n", getpid());
                printf("\nBEFORE PARENT process is %d\n", getppid());
                sleep(5); // Simulate orphan state
                printf("\nAFTER Parent process is %d\n", getppid());
            } else { // Parent process
                printf("Parent process (orphan) is %d\n", getpid());
                exit(0); // Parent exits immediately
            }
            break;

        case 3: // Zombie Process
            if (pid < 0) {
                perror("fork failed");
                exit(1);
            } else if (pid == 0) { // Child process
                printf("\nChild process (Zombie) started.\n");
                sleep(5); // Create zombie process
                printf("Child process (PID: %d) completed.\n", getpid());
            } else { // Parent process
                printf("\nParent process (Zombie) started.\n");
                printf("Parent process will sleep to create a Zombie (PID: %d).\n", pid);
                sleep(10); // Sleep to allow child process to complete
                printf("Parent process (PID: %d) completed.\n", getpid());
                char grepCommand[100];
                snprintf(grepCommand, sizeof(grepCommand), "ps -elf | grep %d", pid);
                system(grepCommand); // Display zombie status
            }
            break;

        default:
            printf("Invalid choice.\n");
            break;
    }

    return 0;
}
