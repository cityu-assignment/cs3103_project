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

#include <stdbool.h>

#define THREAD_NUM 1

sem_t semEmpty;
sem_t semFull;

pthread_mutex_t mutexBuffer;

char buf[255];

int array[10000][2];

int onePart = 0;
char filename[255];

bool inRange(unsigned low, unsigned high, unsigned x);
void parseTime(char * sec);
void printArray(int arr[][2], int n);
int compare( const void* a, const void* b);

void * producer(void* args) {
    char input[25];
    int startAt = (int *)args;
    int endAt = startAt + onePart;
    printf("start: %d, end: %d\n", startAt, endAt);
    
    FILE * file = fopen (filename, "r");
    while(fgets(input,sizeof(input),file)) {    
        for (int i = startAt; i < endAt; i++) { 
            if (!(inRange(array[i][0], array[i+1][0], atoi(strtok(input, ","))))) continue;
            array[i][1] += 1;
            break;
        }
    }
    fclose(file);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) 
{
    clock_t start_time = clock();
    // initialization
    pthread_t th[THREAD_NUM];
    
    pthread_mutex_init(&mutexBuffer, NULL);
    
    sem_init(&semEmpty, 0, 1);
    sem_init(&semFull, 0, 0);
    
    // take inputs
    strcpy(filename, "input.txt");
//     snprintf(buf,sizeof(buf),"%s%s%s", "./", argv[1], "input0");

    int start = 1645491600;
//     int start = atoi(argv[2]);
    const int end = 1679046032;

    const int endTimes = 9312;
    
    int printTimes = 5;
//     int printTimes = atoi(argv[3]);
    
    onePart = endTimes / THREAD_NUM;

    for (int i = 0; start < end; i++, start += 3600){
        array[i][0] = start;
        array[i][1] = 0;
    }
    start = 1645491600;
    
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        int t = (0 + (i * onePart));
        pthread_create(&th[i], NULL, &producer, t);
    }
    
    for (i = 0; i < THREAD_NUM; i++) {
        pthread_join(th[i], NULL);
    }
    
    qsort(array, 9312, sizeof(int*), compare);
    printArray(array, printTimes);
    
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    
  double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
  printf("Done in %f seconds\n", elapsed_time);
    return 0;
}

bool inRange(unsigned low, unsigned high, unsigned x) {
    return (x >= low && x < high);	
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
