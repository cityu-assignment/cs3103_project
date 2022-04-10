#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>

bool inRange(unsigned low, unsigned high, unsigned x) {	
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

void parseTime(char * sec) {
    struct tm tm;
    char buf[25];

    memset(&tm, 0, sizeof(struct tm));
    strptime(sec, "%s", &tm);
    strftime(buf, sizeof(buf), "%a %b %d %H:%M:%S %Y", &tm);
    puts(buf);
}

// Print an array
void printArray(int arr[], int arr2[], int n) {
    char secString[20];
    
    for (int i = 0; i < n; ++i) {
        printf("Access Times: %d, ", arr[i]);
        printf("Time Stamp: %d, ", arr2[i]);
        sprintf(secString, "%d", arr2[i]);
        parseTime(secString);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    char input[255];
    FILE* file = fopen ("input.txt", "r");
    int timeStamp = 0;
    int arraySize = 0;
    
    // args[1]
    int start = 1645491600;
    const int end = 1679046032;
    // args[2]
    int printTimes = 0;
    
    bool didInRangeBefore = false;

    int timeStamps[10000];
    int hours[10000];

    for (int startTime = start; startTime < end; startTime+=3600) {
        
        if (didInRangeBefore) arraySize++;
        didInRangeBefore = false;
        
        file = fopen ("input.txt", "r");
        
        while(fgets(input,sizeof(input),file)) {    
            timeStamp = atoi(strtok(input, ","));
            
            if (!(inRange(startTime, startTime+3600, timeStamp))) continue; 
            didInRangeBefore = true;
            timeStamps[arraySize] = startTime;
            hours[arraySize]++;
        }
    }

    heapSort(hours, timeStamps, arraySize);
    printArray(hours, timeStamps, arraySize);
    
    fclose (file);
    
    return 0;
}
