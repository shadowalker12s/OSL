#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define BUFFER_SIZE 5
#define NUM_READERS 3
#define NUM_WRITERS 2

int buffer[BUFFER_SIZE];
int in = 0, out = 0;
int read_count = 0;

sem_t empty, full;
pthread_mutex_t mutex, read_count_mutex;

void print_buffer() {
    printf("Buffer: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%d", buffer[i]);
        if (i < BUFFER_SIZE - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

void *reader(void *arg) {
    int reader_id = *((int *)arg);
    while (1) {
        // Reader Priority: Increase read count to signal readers are active
        pthread_mutex_lock(&read_count_mutex);
        read_count++;
        if (read_count == 1) {
            // First reader locks the buffer
            pthread_mutex_lock(&mutex);
        }
        pthread_mutex_unlock(&read_count_mutex);

        // Critical Section: Reading from buffer
        int data = buffer[out];
        printf("Reader %d reads: %d\n", reader_id, data);
        print_buffer();

        // End of critical section for reader
        pthread_mutex_lock(&read_count_mutex);
        read_count--;
        if (read_count == 0) {
            // Last reader unlocks the buffer
            pthread_mutex_unlock(&mutex);
        }
        pthread_mutex_unlock(&read_count_mutex);

        sleep(2); // Simulate reading time
    }
    pthread_exit(NULL);
}

void *writer(void *arg) {
    int writer_id = *((int *)arg);
    while (1) {
        int data = rand() % 100; // Generate random data

        // Writer waits for exclusive access to buffer
        pthread_mutex_lock(&mutex);
        
        // Critical Section: Writing to buffer
        buffer[in] = data;
        printf("Writer %d writes: %d\n", writer_id, data);
        in = (in + 1) % BUFFER_SIZE;
        print_buffer();

        // Release exclusive access
        pthread_mutex_unlock(&mutex);

        sleep(5); // Simulate writing time
    }
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL)); // Seed the random number generator
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];

    // Initialize mutexes and semaphores
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&read_count_mutex, NULL);

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    // Destroy mutexes and semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&read_count_mutex);

    return 0;
}
