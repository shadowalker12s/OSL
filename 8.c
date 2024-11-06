#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Function to find the closest track to the current head position for SSTF
int findClosestTrack(int tracks[], int n, int head) {
    int minDist = INT_MAX, index = -1;
    for (int i = 0; i < n; i++) {
        int dist = abs(tracks[i] - head);
        if (tracks[i] >= 0 && dist < minDist) {
            minDist = dist;
            index = i;
        }
    }
    return index;
}

// FCFS Disk Scheduling Algorithm
void fcfs(int tracks[], int n, int head) {
    int totalSeekTime = 0;
    printf("FCFS Disk Scheduling:\n");
    for (int i = 0; i < n; i++) {
        int seek = abs(tracks[i] - head);
        totalSeekTime += seek;
        printf("Move head from %d to %d (Seek %d)\n", head, tracks[i], seek);
        head = tracks[i];
    }
    printf("Total Seek Time: %d\n", totalSeekTime);
}

// SSTF Disk Scheduling Algorithm
void sstf(int tracks[], int n, int head) {
    int totalSeekTime = 0;
    printf("SSTF Disk Scheduling:\n");
    for (int i = 0; i < n; i++) {
        int closestTrackIndex = findClosestTrack(tracks, n, head);
        int seek = abs(tracks[closestTrackIndex] - head);
        totalSeekTime += seek;
        printf("Move head from %d to %d (Seek %d)\n", head, tracks[closestTrackIndex], seek);
        head = tracks[closestTrackIndex];
        tracks[closestTrackIndex] = -1; // Mark track as visited
    }
    printf("Total Seek Time: %d\n", totalSeekTime);
}

// Integer comparison function for qsort (sorting integers)
int compareInt(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// SCAN Disk Scheduling Algorithm
void scan(int tracks[], int n, int head) {
    int totalSeekTime = 0;
    printf("SCAN Disk Scheduling:\n");

    // Sort tracks using the compareInt function
    qsort(tracks, n, sizeof(int), compareInt);

    // Move towards higher numbers first
    int i;
    for (i = 0; i < n && tracks[i] < head; i++);
    for (int j = i; j < n; j++) {
        printf("Move head from %d to %d (Seek %d)\n", head, tracks[j], abs(tracks[j] - head));
        totalSeekTime += abs(tracks[j] - head);
        head = tracks[j];
    }

    // Move back towards lower numbers
    for (int j = i - 1; j >= 0; j--) {
        printf("Move head from %d to %d (Seek %d)\n", head, tracks[j], abs(tracks[j] - head));
        totalSeekTime += abs(tracks[j] - head);
        head = tracks[j];
    }

    printf("Total Seek Time: %d\n", totalSeekTime);
}

// C-SCAN Disk Scheduling Algorithm
void cscan(int tracks[], int n, int head) {
    int totalSeekTime = 0;
    printf("C-SCAN Disk Scheduling:\n");

    // Sort tracks using the compareInt function
    qsort(tracks, n, sizeof(int), compareInt);

    // Move towards higher numbers
    int i;
    for (i = 0; i < n && tracks[i] < head; i++);
    for (int j = i; j < n; j++) {
        printf("Move head from %d to %d (Seek %d)\n", head, tracks[j], abs(tracks[j] - head));
        totalSeekTime += abs(tracks[j] - head);
        head = tracks[j];
    }

    // Wrap around to the beginning of the disk
    if (i > 0) {
        printf("Move head from %d to 0 (Seek %d)\n", head, head);
        totalSeekTime += head;
        head = 0;

        for (int j = 0; j < i; j++) {
            printf("Move head from %d to %d (Seek %d)\n", head, tracks[j], abs(tracks[j] - head));
            totalSeekTime += abs(tracks[j] - head);
            head = tracks[j];
        }
    }

    printf("Total Seek Time: %d\n", totalSeekTime);
}

// LOOK Disk Scheduling Algorithm
void look(int tracks[], int n, int head) {
    int totalSeekTime = 0;
    printf("LOOK Disk Scheduling:\n");

    // Sort tracks using the compareInt function
    qsort(tracks, n, sizeof(int), compareInt);

    // Move towards higher numbers first
    int i;
    for (i = 0; i < n && tracks[i] < head; i++);
    for (int j = i; j < n; j++) {
        printf("Move head from %d to %d (Seek %d)\n", head, tracks[j], abs(tracks[j] - head));
        totalSeekTime += abs(tracks[j] - head);
        head = tracks[j];
    }

    // Move back towards lower numbers
    for (int j = i - 1; j >= 0; j--) {
        printf("Move head from %d to %d (Seek %d)\n", head, tracks[j], abs(tracks[j] - head));
        totalSeekTime += abs(tracks[j] - head);
        head = tracks[j];
    }

    printf("Total Seek Time: %d\n", totalSeekTime);
}

// C-LOOK Disk Scheduling Algorithm
void clook(int tracks[], int n, int head) {
    int totalSeekTime = 0;
    printf("C-LOOK Disk Scheduling:\n");

    // Sort tracks using the compareInt function
    qsort(tracks, n, sizeof(int), compareInt);

    // Move towards higher numbers
    int i;
    for (i = 0; i < n && tracks[i] < head; i++);
    for (int j = i; j < n; j++) {
        printf("Move head from %d to %d (Seek %d)\n", head, tracks[j], abs(tracks[j] - head));
        totalSeekTime += abs(tracks[j] - head);
        head = tracks[j];
    }

    // Wrap around to the lowest request
    if (i > 0) {
        for (int j = 0; j < i; j++) {
            printf("Move head from %d to %d (Seek %d)\n", head, tracks[j], abs(tracks[j] - head));
            totalSeekTime += abs(tracks[j] - head);
            head = tracks[j];
        }
    }

    printf("Total Seek Time: %d\n", totalSeekTime);
}

int main() {
    int n, head, choice;
    printf("Enter the number of tracks: ");
    scanf("%d", &n);
    int tracks[n];
    printf("Enter the track positions:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &tracks[i]);
    }
    printf("Enter the initial head position: ");
    scanf("%d", &head);

    while (1) {
        printf("\nDisk Scheduling Algorithms Menu:\n");
        printf("1. FCFS Disk Scheduling\n");
        printf("2. SSTF Disk Scheduling\n");
        printf("3. SCAN Disk Scheduling\n");
        printf("4. C-SCAN Disk Scheduling\n");
        printf("5. LOOK Disk Scheduling\n");
        printf("6. C-LOOK Disk Scheduling\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fcfs(tracks, n, head);
                break;
            case 2:
                sstf(tracks, n, head);
                break;
            case 3:
                scan(tracks, n, head);
                break;
            case 4:
                cscan(tracks, n, head);
                break;
            case 5:
                look(tracks, n, head);
                break;
            case 6:
                clook(tracks, n, head);
                break;
            case 7:
                exit(0);
            default:
                printf("Invalid choice. Please select a valid option.\n");
                break;
        }
    }
    return 0;
}