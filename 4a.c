#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
sem_t empty, full;
pthread_mutex_t mutex;
int in = 0, out = 0;

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

void *producer(void *arg) {
    int item;
    while (1) {
        // Produce an item (random number)
        item = rand() % 100;

        // Wait if the buffer is full
        sem_wait(&empty);

        // Lock the buffer for safe access
        pthread_mutex_lock(&mutex);

        // Insert the item into the buffer
        buffer[in] = item;
        printf("Producer: %d Produced: %d\n", in, item);
        in = (in + 1) % BUFFER_SIZE;

        // Unlock the buffer
        pthread_mutex_unlock(&mutex);

        // Signal that a new item is available
        sem_post(&full);

        print_buffer();
        sleep(2); // Simulate production time
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int item;
    while (1) {
        // Wait for an item to be available
        sem_wait(&full);

        // Lock the buffer for safe access
        pthread_mutex_lock(&mutex);

        // Remove the item from the buffer
        item = buffer[out];
        printf("Consumer: %d Consumed: %d\n", out, item);
        out = (out + 1) % BUFFER_SIZE;

        // Unlock the buffer
        pthread_mutex_unlock(&mutex);

        // Signal that a slot is now empty
        sem_post(&empty);

        print_buffer();
        sleep(5); // Simulate consumption time
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for both threads to complete
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy semaphores and mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
