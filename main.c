#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <errno.h>

#define THREAD_NUM 40
#define BUFFER_SIZE 65268

FILE * file;
char filename[255];

pthread_t th[THREAD_NUM];

sem_t semEmpty, semFull;

pthread_mutex_t mutexBuffer;

int buffer[BUFFER_SIZE];
int count = 0;

int array[9324][2];
int onePart = 0;

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
    for (int i = 0; i < n; ++i) {
        sprintf(secString, "%d", arr[i][0]);
        parseTime(secString);
        printf("        %d\n", arr[i][1]);
    }
    printf("\n");
}

int compare(const void* a, const void* b) {
    int* x = (int*) a;
    int* y = (int*) b;
    
    if(y[1] == x[1]) return y[0] - x[0];
    return y[1] - x[1];
 }

void* producer(void* args) {
    char input[25];
    
    while(fgets(input,sizeof(input),file)) {
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);
  
        buffer[count] = atoi(strtok(input, ","));
        count++;
        
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);
    }
    
    fclose(file);
    
    while (count != 0);
    
    for (int i = 1; i <= (THREAD_NUM - 1); i++) pthread_cancel(th[i]);
    
    return 0;
}

void* consumer(void* args) {
    int startAt = (int *)args;
    int endAt = startAt + onePart;
    int i = 0;
    int min = 0;
    int max = 0;
    
    while (1) {
        // Remove from the buffer
        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);

        for (i = 0; startAt < endAt; i++) { 
            min = array[i][0];
            max = array[i+1][0];
            if ((unsigned)(buffer[count - 1]-min) <= (max-min)) break;
        }
        
        count--;
        
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);
        
        // Consume
        array[i][1]++;
    }
    
    return 0;
}

int main(int argc, char* argv[]) {
    // take inputs
    char buf[256];
    snprintf(buf,sizeof(buf),"%s%s%s", "./", argv[1], "input0");
    file = fopen(buf, "r");
    
    int start = 1645491600;
    
    for (int i = 0; start <= 1679046032; start += 3600) array[i++][0] = start;
    
    onePart = BUFFER_SIZE / (THREAD_NUM - 1);
    int printTimes = 5;
    
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 1, 9324);
    sem_init(&semFull, 1, 0);
    
    int i;
    int at = 0;
    for (i = 0; i < THREAD_NUM; i++) {
        if (i > 0) {
            pthread_create(&th[i], NULL, &consumer, at += onePart);
        } else {
            pthread_create(&th[i], NULL, &producer, NULL);
        }
    }
    
    for (i = 0; i < THREAD_NUM; i++) pthread_join(th[i], NULL);
    
    printf("Top K frequently accessed hour:\n");
    qsort(array, 9324, sizeof(int*), compare);
    printArray(array, atoi(argv[3]));
    
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    
    return 0;
}
