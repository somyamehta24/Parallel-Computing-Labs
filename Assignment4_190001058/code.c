// 190001058

// Somya Mehta

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define NUM_OF_READERS 10
#define NUM_OF_WRITERS 5
// semaphore
sem_t writer_semaphore;
pthread_mutex_t mutex;

int id_of_reader = 0;
int counti = 1;

void *readVariable(void *read_id) {
    // Reader acquire the lock before modifying id_of_reader
    pthread_mutex_lock(&mutex);
    id_of_reader++;
    if (id_of_reader == 1) {
        sem_wait(&writer_semaphore);  // If this id is of the first reader, then it will block the writer
    }
    pthread_mutex_unlock(&mutex);
    // Reading Section
    printf("Reader %d: read counti as %d\n", *((int *)read_id), counti);

    // Reader acquire the lock before modifying numreader
    pthread_mutex_lock(&mutex);
    id_of_reader--;
    if (id_of_reader == 0) {
        sem_post(&writer_semaphore);  // If this is the last reader, it will wake up the writer.
    }
    pthread_mutex_unlock(&mutex);
}
void *writeVariable(void *writer_id) {
    // Writer acquire the lock before modifying counti
    sem_wait(&writer_semaphore);
    counti = counti * 2;
    printf("Writer Number %d modified counti to %d\n", (*((int *)writer_id)), counti);
    sem_post(&writer_semaphore);
}

int main() {
    // 10 readers and 5 writers
    pthread_t read[NUM_OF_READERS], write[NUM_OF_WRITERS];
    pthread_mutex_init(&mutex, NULL);   // initializing mutex(binary semaphore)
    sem_init(&writer_semaphore, 0, 1);  // initializing semaphore

    int reader_id[NUM_OF_READERS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Creating different threads for readers and writers by passing correspnding variables
    for (int i = 0; i < NUM_OF_READERS; i++) {
        pthread_create(&read[i], NULL, (void *)readVariable, (void *)&reader_id[i]);
    }
    for (int i = 0; i < NUM_OF_WRITERS; i++) {
        pthread_create(&write[i], NULL, (void *)writeVariable, (void *)&reader_id[i]);
    }
    // Joining the created  threads for readers and writers
    for (int i = 0; i < NUM_OF_READERS; i++) {
        pthread_join(read[i], NULL);
    }
    for (int i = 0; i < NUM_OF_WRITERS; i++) {
        pthread_join(write[i], NULL);
    }
    // This destroys the mutex
    pthread_mutex_destroy(&mutex);
    // This destroys the semaphore
    sem_destroy(&writer_semaphore);

    return 0;
}
