#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0; // Number of items in buffer

pthread_mutex_t mutex;
pthread_cond_t cond_full;
pthread_cond_t cond_empty;

// Producer function
void* producer(void* arg) {
    int id = *(int*)arg;
    int item = 1;

    while (item <= 5) { // Each producer produces 5 items
        pthread_mutex_lock(&mutex);

        // Wait if buffer is full
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&cond_full, &mutex);
        }

        // Add item to buffer
        buffer[count] = item;
        count++;
        printf("Producer %d produced %d\n", id, item);

        // Signal consumer that buffer is not empty
        pthread_cond_signal(&cond_empty);

        pthread_mutex_unlock(&mutex);

        item++;
        sleep(1); // Small delay
    }

    return NULL;
}

// Consumer function
void* consumer(void* arg) {
    int item;

    while (1) {
        pthread_mutex_lock(&mutex);

        // Wait if buffer is empty
        while (count == 0) {
            pthread_cond_wait(&cond_empty, &mutex);
        }

        // Remove item from buffer
        item = buffer[count - 1];
        count--;
        printf("Consumer consumed %d\n", item);

        // Signal producers that buffer has space
        pthread_cond_signal(&cond_full);

        pthread_mutex_unlock(&mutex);

        sleep(2); // Small delay
    }

    return NULL;
}

int main() {
    pthread_t prod1, prod2, cons;
    int id1 = 1, id2 = 2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_full, NULL);
    pthread_cond_init(&cond_empty, NULL);

    pthread_create(&prod1, NULL, producer, &id1);
    pthread_create(&prod2, NULL, producer, &id2);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod1, NULL);
    pthread_join(prod2, NULL);
    // Consumer runs infinitely, you may cancel it after producers finish

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_full);
    pthread_cond_destroy(&cond_empty);

    return 0;
}
