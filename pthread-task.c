#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Declare a mutex
pthread_mutex_t lock;

// Thread function
void* threadFunction(void* arg) {
    int thread_id = *(int*)arg;

    for (int i = 0; i < 3; i++) {
        // Lock the mutex before printing
        pthread_mutex_lock(&lock);

        printf("Thread %d: Message %d\n", thread_id, i + 1);

        // Unlock the mutex after printing
        pthread_mutex_unlock(&lock);

        sleep(1); // Small delay for clarity
    }

    return NULL;
}

int main() {
    pthread_t threads[3];
    int thread_ids[3] = {1, 2, 3};

    // Initialize mutex
    pthread_mutex_init(&lock, NULL);

    // Create 3 threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, threadFunction, &thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&lock);

    printf("All threads have finished execution.\n");

    return 0;
}
