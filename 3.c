#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    int pid;
    int burst_time;
    int arrival_time;
    int waiting_time;
    int turnaround_time;
    int remaining_time;
    int priority;
} Process;

void print_gantt_chart(Process p[], int n);
void fcfs(Process p[], int n);
void sjf_non_preemptive(Process p[], int n);
void sjf_preemptive(Process p[], int n);
void priority_non_preemptive(Process p[], int n);
void priority_preemptive(Process p[], int n);
void round_robin(Process p[], int n, int quantum);

void fcfs(Process p[], int n) {
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time)
            current_time = p[i].arrival_time;
        p[i].waiting_time = current_time - p[i].arrival_time;
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
        current_time += p[i].burst_time;
    }
}

void sjf_non_preemptive(Process p[], int n) {
    int current_time = 0, completed = 0;
    while (completed != n) {
        int idx = -1, min_burst_time = __INT_MAX__;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0 && p[i].burst_time < min_burst_time) {
                min_burst_time = p[i].burst_time;
                idx = i;
            }
        }
        if (idx == -1) {
            current_time++;
            continue;
        }
        current_time += p[idx].burst_time;
        p[idx].waiting_time = current_time - p[idx].arrival_time - p[idx].burst_time;
        p[idx].turnaround_time = p[idx].waiting_time + p[idx].burst_time;
        p[idx].remaining_time = 0;
        completed++;
    }
}

void sjf_preemptive(Process p[], int n) {
    int completed = 0, current_time = 0;
    while (completed != n) {
        int idx = -1, min_burst_time = __INT_MAX__;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0 && p[i].remaining_time < min_burst_time) {
                min_burst_time = p[i].remaining_time;
                idx = i;
            }
        }
        if (idx == -1) {
            current_time++;
            continue;
        }
        p[idx].remaining_time--;
        current_time++;
        if (p[idx].remaining_time == 0) {
            completed++;
            p[idx].waiting_time = current_time - p[idx].arrival_time - p[idx].burst_time;
            p[idx].turnaround_time = p[idx].waiting_time + p[idx].burst_time;
        }
    }
}

void priority_non_preemptive(Process p[], int n) {
    int current_time = 0, completed = 0;
    while (completed != n) {
        int idx = -1, highest_priority = __INT_MAX__;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0 && p[i].priority < highest_priority) {
                highest_priority = p[i].priority;
                idx = i;
            }
        }
        if (idx == -1) {
            current_time++;
            continue;
        }
        current_time += p[idx].burst_time;
        p[idx].waiting_time = current_time - p[idx].arrival_time - p[idx].burst_time;
        p[idx].turnaround_time = p[idx].waiting_time + p[idx].burst_time;
        p[idx].remaining_time = 0;
        completed++;
    }
}

void priority_preemptive(Process p[], int n) {
    int current_time = 0, completed = 0;
    while (completed != n) {
        int idx = -1, highest_priority = __INT_MAX__;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0 && p[i].priority < highest_priority) {
                highest_priority = p[i].priority;
                idx = i;
            }
        }
        if (idx == -1) {
            current_time++;
            continue;
        }
        p[idx].remaining_time--;
        current_time++;
        if (p[idx].remaining_time == 0) {
            completed++;
            p[idx].waiting_time = current_time - p[idx].arrival_time - p[idx].burst_time;
            p[idx].turnaround_time = p[idx].waiting_time + p[idx].burst_time;
        }
    }
}

void round_robin(Process p[], int n, int quantum) {
    int remaining_processes = n;
    int current_time = 0;

    while (remaining_processes > 0) {
        for (int i = 0; i < n; i++) {
            if (p[i].remaining_time > 0) {
                if (p[i].remaining_time <= quantum) {
                    current_time += p[i].remaining_time;
                    p[i].remaining_time = 0;
                    p[i].waiting_time = current_time - p[i].arrival_time - p[i].burst_time;
                    p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
                    remaining_processes--;
                } else {
                    current_time += quantum;
                    p[i].remaining_time -= quantum;
                }
            }
        }
    }
}

void print_gantt_chart(Process p[], int n) {
    int current_time = 0;
    printf("Gantt Chart: ");
    for (int i = 0; i < n; i++) {
        printf(" |%d P%d %d", current_time,p[i].pid,current_time+p[i].burst_time);
        current_time += p[i].burst_time;
    }
    printf("|\n");
}

int main() {
    Process p[MAX];
    int n, quantum, choice;

    printf("Enter total number of processes: ");
    scanf("%d", &n);
    printf("Enter the Arrival Time, Burst Time, and Priority:\n");
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("P[%d] Arrival Time: ", i + 1);
        scanf("%d", &p[i].arrival_time);
        printf("P[%d] Burst Time: ", i + 1);
        scanf("%d", &p[i].burst_time);
        printf("P[%d] Priority: ", i + 1);
        scanf("%d", &p[i].priority);
        p[i].remaining_time = p[i].burst_time;
    }

    printf("\n\nMENU:\n");
    printf("1. FCFS\n");
    printf("2. SJF Non-Preemptive\n");
    printf("3. SJF Preemptive\n");
    printf("4. Priority Non-Preemptive\n");
    printf("5. Priority Preemptive\n");
    printf("6. Round Robin\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            fcfs(p, n);
            break;
        case 2:
            sjf_non_preemptive(p, n);
            break;
        case 3:
            sjf_preemptive(p, n);
            break;
        case 4:
            priority_non_preemptive(p, n);
            break;
        case 5:
            priority_preemptive(p, n);
            break;
        case 6:
            printf("Enter the time quantum: ");
            scanf("%d", &quantum);
            round_robin(p, n, quantum);
            break;
        default:
            printf("Invalid choice.\n");
            return 0;
    }

    double total_waiting_time = 0, total_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        total_waiting_time += p[i].waiting_time;
        total_turnaround_time += p[i].turnaround_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    print_gantt_chart(p, n);

    return 0;
}
