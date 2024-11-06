#include <stdio.h>

void fcfs(int pages[], int n, int frames, int *hit, int *fault) {
    int frame[frames], i, j, k;
    for (i = 0; i < frames; i++)
        frame[i] = -1;
    j = 0;

    printf("\nRef String\tPage Frames\n");
    for (i = 0; i < n; i++) {
        int flag = 0;
        for (k = 0; k < frames; k++) {
            if (frame[k] == pages[i]) {
                flag = 1;
                (*hit)++;
                break;
            }
        }
        if (flag == 0) {
            frame[j] = pages[i];
            j = (j + 1) % frames;
            (*fault)++;
        }

        printf("\n%d\t\t", pages[i]);
        for (k = 0; k < frames; k++) {
            if (frame[k] == -1)
                printf("*\t");
            else
                printf("%d\t", frame[k]);
        }
    }
    printf("\n\nTotal Page Faults: %d", *fault);
}

void lru(int pages[], int n, int frames, int *hit, int *fault) {
    int frame[frames], lru[frames], i, j, k;
    for (i = 0; i < frames; i++) {
        frame[i] = -1;
        lru[i] = frames - i;
    }

    printf("\nRef String\tPage Frames\n");
    for (i = 0; i < n; i++) {
        int flag = 0;
        for (k = 0; k < frames; k++) {
            if (frame[k] == pages[i]) {
                flag = 1;
                (*hit)++;
                lru[k] = frames - 1;
                break;
            }
        }
        if (flag == 0) {
            int min = 0;
            for (k = 0; k < frames; k++) {
                if (lru[k] < lru[min])
                    min = k;
            }
            frame[min] = pages[i];
            lru[min] = frames - 1;
            (*fault)++;
        }

        for (j = 0; j < frames; j++)
            lru[j]--;

        printf("\n%d\t\t", pages[i]);
        for (k = 0; k < frames; k++) {
            if (frame[k] == -1)
                printf("*\t");
            else
                printf("%d\t", frame[k]);
        }
    }
    printf("\n\nTotal Page Faults: %d", *fault);
}

int findOptimal(int pages[], int n, int frame[], int frames, int startIndex) {
    int res = -1, farthest = startIndex;
    for (int i = 0; i < frames; i++) {
        int j;
        for (j = startIndex; j < n; j++) {
            if (frame[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    res = i;
                }
                break;
            }
        }
        if (j == n)
            return i;
    }
    return (res == -1) ? 0 : res;
}

void optimal(int pages[], int n, int frames, int *hit, int *fault) {
    int frame[frames], i, j, k;
    for (i = 0; i < frames; i++)
        frame[i] = -1;

    printf("\nRef String\tPage Frames\n");
    for (i = 0; i < n; i++) {
        int flag = 0;
        for (k = 0; k < frames; k++) {
            if (frame[k] == pages[i]) {
                flag = 1;
                (*hit)++;
                break;
            }
        }
        if (flag == 0) {
            int pos = findOptimal(pages, n, frame, frames, i + 1);
            frame[pos] = pages[i];
            (*fault)++;
        }

        printf("\n%d\t\t", pages[i]);
        for (k = 0; k < frames; k++) {
            if (frame[k] == -1)
                printf("*\t");
            else
                printf("%d\t", frame[k]);
        }
    }
    printf("\n\nTotal Page Faults: %d", *fault);
}

int main() {
    int choice, refLen, frames, hit = 0, fault = 0;

    printf("Enter length of reference string: ");
    scanf("%d", &refLen);
    int pages[refLen];

    printf("Enter reference string: ");
    for (int i = 0; i < refLen; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Enter frame size (minimum 3): ");
    scanf("%d", &frames);

    if (frames < 3) {
        printf("Frame size must be at least 3.\n");
        return 1;
    }

    do {
        printf("\nMenu:\n");
        printf("1. FIFO\n");
        printf("2. LRU\n");
        printf("3. Optimal\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                hit = 0;
                fault = 0;
                fcfs(pages, refLen, frames, &hit, &fault);
                printf("\nHit: %d, Fault: %d\n", hit, fault);
                break;
            case 2:
                hit = 0;
                fault = 0;
                lru(pages, refLen, frames, &hit, &fault);
                printf("\nHit: %d, Fault: %d\n", hit, fault);
                break;
            case 3:
                hit = 0;
                fault = 0;
                optimal(pages, refLen, frames, &hit, &fault);
                printf("\nHit: %d, Fault: %d\n", hit, fault);
                break;
            case 4:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}
