#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Function to swap two elements in an array
void swap(long long *a, long long *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to randomly select a pivot and partition the array
int partition(long long arr[], int low, int high) {
    // Generate a random index between low and high
    int randomIndex = low + rand() % (high - low + 1);
    
    // Swap the randomly chosen pivot with the last element
    swap(&arr[randomIndex], &arr[high]);

    int pivot = arr[high]; // Pivot element
    int i = (low - 1); // Index of the smaller element

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    // Swap the pivot element with the element at index (i + 1)
    swap(&arr[i + 1], &arr[high]);

    return (i + 1);
}

// Function to perform Quick Sort
void quickSort(long long arr[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);

        // Recursively sort elements before and after the pivot
        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

void populate_array(long long arr[], int size, long long smaller_than) {
    for (int i= 0; i< size; i++) {
        long long number= (long long) rand() % (smaller_than + 1);
        arr[i]= number;
        // printf("number: %llu, arr[%d]: %llu\n", number, i, arr[i]); 
    }
}

int main() {
    // long long arr[] = {12LL, 5LL, 6LL, 8LL, 3LL, 9LL, 7LL, 2LL};
    long long POWER_TWO= exp2(3);
    int n = 10;
    long long arr[n];

    // NOTE THAT THIS PROGRAM DOES NOT WORK WHEN n > POWER_TWO

    populate_array(arr, n, POWER_TWO);

    srand(time(NULL)); // Seed for random number generation

    printf("Original array: ");
    for (int i = 0; i < n; i++) {
        printf("%llu ", arr[i]);
    }

    quickSort(arr, 0, n - 1);

    printf("\nSorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%llu ", arr[i]);
    }

    return 0;
}
