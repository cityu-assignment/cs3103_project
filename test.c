#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include <time.h>
#include <semaphore.h>

#define THREAD_NUM 4

sem_t semEmpty;
sem_t semFull;

pthread_mutex_t mutexBuffer;

// buffer size is max of timestamp array
int buffer[10];
// int timeStamp[] array;
int count = 0;
// int accessCount = 0;
// char secString[0];

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void* producer(void* args) {
    while (1) {
        // Read timestamp one by one {
        // check if it is NOT in range (start to start + 3600 sec) continue;
        // }
        // else
        // not timestamp have to read {
        // unlock mutex:
        // pthread_mutex_unlock(&mutexBuffer);
        // sem_post(&semFull);
        // }
        
        // accessCount++;
        
        // int x = timestamp;
        int x = rand() % 100;

        // Add to the buffer
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);
  
        buffer[count] = x;
        count++;
        
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);
    }
}

void* consumer(void* args) {
    while (1) {
        int y;

        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);
        
        // Critical section
        y = buffer[count - 1];
        count--;
        
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);
        // End critical section
        
        // Consume
        // add y into timeStamp array;
        
        // if y is Empty then
        // heap sort timeStamp array; 
        // and print

        // for (int i = 0; i < accessCount; i++) {
        //     sprintf(secString, "%d", timeStamp[i]);
        //     parseTime(secString);
        // }

        printf("Got %d\n", y);
    }
}
  
void heapify(int arr[], int n, int i) {
    // Find largest among root, left child and right child
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
  
    if (left < n && arr[left] < arr[smallest])
      smallest = left;
  
    if (right < n && arr[right] < arr[smallest])
      smallest = right;
  
    // Swap and continue heapifying if root is not largest
    if (smallest != i) {
      swap(&arr[i], &arr[smallest]);
      heapify(arr, n, smallest);
    }
}
  
// Main function to do heap sort
void heapSort(int arr[], int n) {
    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
  
    // Heap sort
    for (int i = n - 1; i >= 0; i--) {
        swap(&arr[0], &arr[i]);
        // Heapify root element to get highest element at root again
        heapify(arr, i, 0);
    }
}

void parseTime(char * sec) {
    struct tm tm;
    char buf[25];

    memset(&tm, 0, sizeof(struct tm));
    strptime(sec, "%s", &tm);
    strftime(buf, sizeof(buf), "%a %b %d %H:%M:%S %Y", &tm);
    puts(buf);
}

int main(int argc, char *argv[]) 
{
    srand(time(NULL));
    
    pthread_t th[THREAD_NUM];
    
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, 10);
    sem_init(&semFull, 0, 0);
    
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (i > 0) {
            if (pthread_create(&th[i], NULL, &producer, NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &consumer, NULL) != 0) {
                perror("Failed to create thread");
            }
        }
    }
    
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    
    return 0;
}
