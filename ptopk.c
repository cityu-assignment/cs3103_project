#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(int arr[][2], int n, int i) {
    // Find largest among root, left child and right child
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
  
    if (left < n && arr[left][1] < arr[smallest][1])
      smallest = left;
  
    if (right < n && arr[right][1] < arr[smallest][1])
      smallest = right;
  
    // Swap and continue heapifying if root is not largest
    if (smallest != i) {
      swap(&arr[i][0], &arr[smallest][0]);
      swap(&arr[i][1], &arr[smallest][1]);
      heapify(arr, n, smallest);
    }
}
  
// Main function to do heap sort
void heapSort(int arr[][2], int n) {
    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
  
    // Heap sort
    for (int i = n - 1; i >= 0; i--) {
        swap(&arr[0][0], &arr[i][0]);
        swap(&arr[0][1], &arr[i][1]);
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

void printArray(int arr[][2], int n) {
    char secString[20];
    
    for (int i = 0; i < n; ++i) {
        printf("Access Times: %d, ", arr[i][1]);
        printf("Time Stamp: %d, ", arr[i][0]);
        sprintf(secString, "%d", arr[i][0]);
        parseTime(secString);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    char *test;
    int a = 0;
    char input[255];
    FILE* file = fopen ("case1/input1", "r");
    char c[20];
    int timeStamp = 0;
    int arraySize = 0;
    
    int start = 1645491600;
    int end = start+3600;
    int array[10000][2];
    for (int i = 0; i < 9311; i++){
        array[i][0] = start;
        array[i][1] = 0;
        start += 3600;
    }
    start = 1645491600;
    
    //while (!feof (file)) 
    //{
    //    fscanf (file, "%d %s", &timeStamp, c);  
    //    if (!(timeStamp >= start && timeStamp <= end)) continue;
    //    array[arraySize++] = timeStamp;
    //}
    
    for (int i =0; i < 9311; i++){
        while(fgets(input,sizeof(input),file)){            
            test = strtok(input, ",");
            a = atoi(test);
            //printf("%d\n",a);
            if (a >= array[i][0] && a < array[i+1][0]){
                array[i][1] += 1;
            }
        }
    }
    
 
    // i actually is array size
    heapSort(array, arraySize);

    printArray(array, 9311);
    
    fclose (file);
    return 0;
}
