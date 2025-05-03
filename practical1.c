#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

#define PthreadSize 3
int *array = NULL;
int size = 0;

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void* partialSort(void* arg)
 {
    long long start, end;
    int thread_id = *(int*)arg;
    start = (size / 2) * thread_id;
    end = (thread_id == PthreadSize - 1) ? size : start + (size / 2);
    
    int i, j;
    int swapped;
    
    for (i = start; i < end - 1; i++)
    {
        swapped = 0; 
        for (j = start; j < end - i - 1 + start; j++)
        {
            if (array[j] > array[j+1]) 
            {
                swap(&array[j], &array[j+1]);
                swapped = 1;
            }
        }
        if (swapped == 0)
            break;
    }
    pthread_exit(NULL);
}

void* mergeParts(void* arg) 
{
    int *temp = (int*)malloc(size * sizeof(int));
    int i = 0, j = size / 2, k = 0;
    
    while (i < size / 2 && j < size) 
    {
        if (array[i] <= array[j])
            temp[k++] = array[i++];
        else
            temp[k++] = array[j++];
    }
    
    while (i < size / 2) 
        temp[k++] = array[i++];
    
    while (j < size) 
        temp[k++] = array[j++];
    
    for (i = 0; i < size; i++) 
        array[i] = temp[i];
    
    free(temp);
    pthread_exit(NULL);
}

void printArray() 
{
    printf("Sorted array: ");
    for (int i = 0; i < size; i++) 
        printf("%d ", array[i]);
    printf("\n");
}

int main() {
    int capacity = 0;
    int input;
    
    printf("Please input your list:\n");
    while (scanf("%d", &input) == 1) 
    {
        if (size >= capacity) 
        {
            capacity = capacity == 0 ? 1 : capacity * 2;
            array = (int *)realloc(array, capacity * sizeof(int));
            if (array == NULL)
            {
                printf("Memory allocation failed\n");
                return 1;
            }
        }
        array[size++] = input;
    }
    
    if (size == 0) 
    {
        printf("No input provided.\n");
        return 0;
    }
    
    pthread_t threads[PthreadSize];
    int thread_ids[PthreadSize];
    
    for (int i = 0; i < 2; i++) 
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, partialSort, &thread_ids[i]);
    }
    
    for (int i = 0; i < 2; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    thread_ids[2] = 2;
    pthread_create(&threads[2], NULL, mergeParts, &thread_ids[2]);
    pthread_join(threads[2], NULL);
    
    printArray();
    
    free(array);
    return 0;
}