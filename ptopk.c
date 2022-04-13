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

#define THREAD_NUM 6
#define BUFFER_SIZE 9312

sem_t semEmpty;
sem_t semFull;

pthread_mutex_t mutexBuffer;

int buffer[BUFFER_SIZE];
int count = 0;

int array[9312][2];
const int endAt = 1679046032;

FILE * file;
char filename[255];
pthread_t th[THREAD_NUM];

void parseTime(char * sec);
void printArray(int arr[][2], int n);
int compare( const void* a, const void* b);

void* producer(void* args) {
    char input[25];
    int time = 0;
    
    while(fgets(input,sizeof(input),file)) {
        time = atoi(strtok(input, ","));
        // Add to the buffer
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);
  
        buffer[count] = time;
        count++;
        
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);
    }
    
    pthread_exit(NULL);
}

void* consumer(void* args) {
    int i = 0;
    while (1)  {
        int time = 0;

        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);
        
        // Critical section
        time = buffer[count - 1];
        count--;
        
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);
        // End critical section
        
        // Consume
        for (int startTime = 1645491600, i = 0; startTime < endAt; startTime+=3600, i++) { 
            if (!(time >= startTime && time < startTime + 3600)) continue; 
            array[i][0] = startTime;
            array[i][1] += 1;
            break;
        }
        i++;
        printf("i: %d\n", i);
        // break;
        if ((feof(file)) && count == 0) break;
    }
}

int main(int argc, char *argv[]) 
{
    clock_t start_time = clock();
    
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, BUFFER_SIZE);
    sem_init(&semFull, 0, 0);
    char buf[255];
    // take inputs
    //strcpy(filename, "input.txt");
    snprintf(buf,sizeof(buf),"%s%s%s", "./", argv[1], "input0");
    file = fopen(buf, "r");

    //int start = 1645491600;
    int start = atoi(argv[2]);
    
    //int printTimes = 5;
    int printTimes = atoi(argv[3]);
    
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (i > 0) {
            pthread_create(&th[i], NULL, &producer, NULL);
        } else {
            pthread_create(&th[i], NULL, &consumer, NULL);
        }
    }
    
    for (i = 0; i < THREAD_NUM; i++) {
        pthread_join(th[i], NULL);
    }
    
    fclose(file);
    
    qsort(array, 9312, sizeof(int*), compare);
    printArray(array, printTimes);
    
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    
    double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    printf("Done in %f seconds\n", elapsed_time);
    
    return 0;
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
void printArray(int arr[][2], int n) {
    char secString[20];
    
    printf("Top K frequently accessed hour:\n");
    for (int i = 0; i < n; ++i) {
        sprintf(secString, "%d", arr[i][0]);
        parseTime(secString);
        printf("        %d\n", arr[i][1]);
    }
    printf("\n");
}

int compare( const void* a, const void* b) {
    int* x = (int*) a;
    int* y = (int*) b;
    
    if(y[1]==x[1]) return y[0] - x[0];
    return y[1] - x[1];
 }
