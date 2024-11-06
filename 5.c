#include <stdio.h>

int checkRequest(int request[], int avail[], int r) {
    for (int i = 0; i < r; i++) {
        if (request[i] > avail[i]) {
            return 0; // Request cannot be granted as it exceeds available resources
        }
    }
    return 1; // Request can be granted
}

int main() {
    int n, r, i, j, k;
    printf("Enter the number of processes (n): ");
    scanf("%d", &n);
    printf("Enter the number of resources (r): ");
    scanf("%d", &r);

    int alloc[n][r], max[n][r], avail[r];

    // Input Allocation Matrix
    printf("Enter the Allocation Matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < r; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }

    // Input Maximum Matrix
    printf("Enter the MAX Matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < r; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    // Input Available Resources
    printf("Enter the Available Resources:\n");
    for (i = 0; i < r; i++) {
        scanf("%d", &avail[i]);
    }

    // Display the Matrices
    printf("Processes Allocation Max Need Available\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t", i);
        for (j = 0; j < r; j++) {
            printf("%d ", alloc[i][j]);
        }
        printf("\t");
        for (j = 0; j < r; j++) {
            printf("%d ", max[i][j]);
        }
        printf("\t");
        for (j = 0; j < r; j++) {
            printf("%d ", max[i][j] - alloc[i][j]);
        }
        printf("\t");
        if (i == 0) {
            for (j = 0; j < r; j++) {
                printf("%d ", avail[j]);
            }
        }
        printf("\n");
    }

    // Initialize the finish array and safe sequence array
    int f[n], ans[n], ind = 0;
    for (k = 0; k < n; k++) {
        f[k] = 0;
    }

    int need[n][r];
    // Calculate Need Matrix
    for (i = 0; i < n; i++) {
        for (j = 0; j < r; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    // Find Safe Sequence
    int y;
    for (k = 0; k < n; k++) {
        for (i = 0; i < n; i++) {
            if (f[i] == 0) {
                int flag = 0;
                for (j = 0; j < r; j++) {
                    if (need[i][j] > avail[j]) {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0) {
                    ans[ind++] = i;
                    for (y = 0; y < r; y++) {
                        avail[y] += alloc[i][y];
                    }
                    f[i] = 1;
                }
            }
        }
    }

    // Display Safe Sequence
    printf("The Safe Sequence is as follows:\n");
    for (i = 0; i < n - 1; i++) {
        printf("P%d -> ", ans[i]);
    }
    printf("P%d\n", ans[n - 1]);

    // Request additional resources for a process
    int request[r], process;
    printf("Enter the process number (0 to %d) for resource request: ", n - 1);
    scanf("%d", &process);
    if (process < 0 || process >= n) {
        printf("Invalid Process Number.\n");
        return 0;
    }

    printf("Enter the request for resources:\n");
    for (i = 0; i < r; i++) {
        scanf("%d", &request[i]);
    }

    // Check if request can be granted
    if (checkRequest(request, avail, r)) {
        // Grant the request and adjust available, allocated, and need matrices
        for (int i = 0; i < r; i++) {
            avail[i] -= request[i];
            alloc[process][i] += request[i];
            need[process][i] -= request[i];
        }

        // Reset data structures to re-check safe sequence
        for (k = 0; k < n; k++) {
            f[k] = 0;
        }
        ind = 0;

        // Re-run Banker's Algorithm for safe sequence after granting request
        for (k = 0; k < n; k++) {
            for (i = 0; i < n; i++) {
                if (f[i] == 0) {
                    int flag = 0;
                    for (j = 0; j < r; j++) {
                        if (need[i][j] > avail[j]) {
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 0) {
                        ans[ind++] = i;
                        for (y = 0; y < r; y++) {
                            avail[y] += alloc[i][y];
                        }
                        f[i] = 1;
                    }
                }
            }
        }

        // Display Safe Sequence after granting the request
        printf("The Safe Sequence after granting the request is as follows:\n");
        for (i = 0; i < n - 1; i++) {
            printf("P%d -> ", ans[i]);
        }
        printf("P%d\n", ans[n - 1]);
    } else {
        printf("Request exceeds available resources. Cannot Proceed.\n");
    }
    return 0;
}
