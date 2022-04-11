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

pthread_mutex_t mutexBuffer, mutexArraySize;

char input[255];
char buf[255];

int timeStamps[10000];
int hours[10000];
int arraySize = 0;
File * file;

int endThreads = 0;

bool inRange(unsigned low, unsigned high, unsigned x);
void swap(int *a, int *b);
void heapify(int arr[], int arr2[], int n, int i);
void heapSort(int arr[], int arr2[], int n);
void timeSort(int arr[], int arr2[], int n);
void parseTime(char * sec);
void printArray(int arr[], int arr2[], int n);

void* producer(void* args) {
    bool didInRangeBefore = false;
    int timeStamp = 0;

    for (int startTime = start; startTime < end; startTime+=3600) {
        file = fopen (buf, "r");
        
        pthread_mutex_lock(&mutexArraySize);
        
        if (didInRangeBefore) arraySize++;
        didInRangeBefore = false;
        
        pthread_mutex_unlock(&mutexArraySize);
            
        while(fgets(input,sizeof(input),file)) {    
            timeStamp = atoi(strtok(input, ","));

            if (!(inRange(startTime, startTime+3600, timeStamp))) continue; 
            
            // Add to the buffer
            sem_wait(&semEmpty);
            pthread_mutex_lock(&mutexBuffer);
            
            // remove timeStamp from file
            // update file file = 
            
            didInRangeBefore = true;
            timeStamps[arraySize] = startTime;
            hours[arraySize]++;
        
            pthread_mutex_unlock(&mutexBuffer);
            sem_post(&semFull);
        }
        
        fclose (file);

    }
    endThreads++;
}

void* consumer(void* args) {
    while (endThreads < (THREAD_NUM - 1)) {
        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);
 
        // sorting
        heapSort(hours, timeStamps, arraySize);

        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);
        // End critical section
    }
    // print timestamp
    timeSort(hours, timeStamps, arraySize);
    printArray(hours, timeStamps, printTimes);
}

int main(int argc, char *argv[]) 
{
    srand(time(NULL));
    
    snprintf(buf,sizeof(buf),"%s%s%s", "./", argv[1], "input0");
    int start = atoi(argv[2]);
    const int end = 1679046032;
    int printTimes = atoi(argv[3]);
    
    int onePart = (end - start) / (THREAD_NUM - 1);
    
    pthread_t th[THREAD_NUM];
    
    pthread_mutex_init(&mutexBuffer, NULL);
    pthread_mutex_init(&mutexArraySize, NULL);
    
    sem_init(&semEmpty, 0, 10000);
    sem_init(&semFull, 0, 0);
    
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (i > 0) {
            // pass start
            if (pthread_create(&th[i], NULL, &producer, NULL) != 0) {
                perror("Failed to create thread");
            }
            // start += onePart
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
    pthread_mutex_destroy(&mutexArraySize);
    
    return 0;
}

bool inRange(unsigned low, unsigned high, unsigned x) {	https://github.com/CityU-CS-Assignment/CS3103_Project/blob/develop/jack.c
    return (x >= low && x < high);	
}		

// Function to swap the the position of two elements
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
  
void heapify(int arr[], int arr2[], int n, int i) {
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
        swap(&arr2[i], &arr2[smallest]);
        
        heapify(arr, arr2, n, smallest);
    }
}
  
// Main function to do heap sort
void heapSort(int arr[], int arr2[], int n) {
    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, arr2, n, i);
  
    // Heap sort
    for (int i = n - 1; i >= 0; i--) {
        swap(&arr[0], &arr[i]);
        swap(&arr2[0], &arr2[i]);
        // Heapify root element to get highest element at root again
        heapify(arr, arr2, i, 0);
    }
}

void timeSort(int arr[], int arr2[], int n) {
    int i, j;
    for (i = 0; i < n-1; i++)    
     
    // Last i elements are already in place
    for (j = 0; j < n-i-1; j++)
        if (arr[j] == arr[j+1] && arr2[j] < arr2[j+1]) {
            swap(&arr2[j], &arr2[j+1]);
            swap(&arr[j], &arr[j+1]);
        }
}

void parseTime(char * sec) {
    struct tm tm;
    char buf[25];

    memset(&tm, 0, sizeof(struct tm));
    strptime(sec, "%s", &tm);
    strftime(buf, sizeof(buf), "%a %b %-2d %H:%M:%S %Y", &tm);
    printf("%s", buf);
}

// Print an array
void printArray(int arr[], int arr2[], int n) {
    printf("Top K frequently accessed hour:\n");
    char secString[20];
    
    for (int i = 0; i < n; ++i) {
        sprintf(secString, "%d", arr2[i]);
        parseTime(secString);
        printf("        %d\n", arr[i]);
        //sprintf(secString, "%d", arr2[i]);
        //printf("Access Times: %d, ", arr[i]);
        //printf("Time Stamp: %d, ", arr2[i]);
        //sprintf(secString, "%d", arr2[i]);
        
    }
    printf("\n");
}
