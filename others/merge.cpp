#include <thread>
#include <iostream>
#include <mutex>
#include <string>

#define TABLE_SIZE 30
#define NB_THREADS 1

std::mutex mutex;

int active_threads = 0;

// TODO: you can do it without copying the arrays ;)
void merge(int array[], int const left, int const mid, int const right)
{
	int const subArrayOne = mid - left + 1;
	int const subArrayTwo = right - mid;

	// Create temp arrays
	int *leftArray = new int[subArrayOne],
		*rightArray = new int[subArrayTwo];

	// Copy data to temp arrays leftArray[] and rightArray[]
	for (int i = 0; i < subArrayOne; i++)
		leftArray[i] = array[left + i];
	for (int j = 0; j < subArrayTwo; j++)
		rightArray[j] = array[mid + 1 + j];

	int indexOfSubArrayOne = 0; // Initial index of first sub-array
    int indexOfSubArrayTwo = 0; // Initial index of second sub-array
	int indexOfMergedArray = left; // Initial index of merged array

	// Merge the temp arrays back into array[left..right]
	while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
		if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
			array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
			indexOfSubArrayOne++;
		}
		else {
			array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
			indexOfSubArrayTwo++;
		}
		indexOfMergedArray++;
	}
	// Copy the remaining elements of
	// left[], if there are any
	while (indexOfSubArrayOne < subArrayOne) {
		array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
		indexOfSubArrayOne++;
		indexOfMergedArray++;
	}
	// Copy the remaining elements of
	// right[], if there are any
	while (indexOfSubArrayTwo < subArrayTwo) {
		array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
		indexOfSubArrayTwo++;
		indexOfMergedArray++;
	}
}

void mergeSort(int array[], int const begin, int const end)
{
	if (begin >= end)
    {
        return; // 1 item or less, so it's sorted
    }
	int mid = begin + (end - begin) / 2;
    
    mutex.lock();
    int remaining_thread = NB_THREADS - active_threads;
    mutex.unlock();
    if (remaining_thread == 2)
    {
        mutex.lock(); active_threads += 2; mutex.unlock();
        std::thread thread_down = std::thread(mergeSort, array, begin, mid);
        std::thread thread_up = std::thread(mergeSort, array, begin, mid);

        thread_down.join();
        mutex.lock(); active_threads -= 1; mutex.unlock();
        thread_up.join();
        mutex.lock(); active_threads -= 1; mutex.unlock();
    } else if (remaining_thread == 1)
    {
        mutex.lock(); active_threads += 2; mutex.unlock();
        std::thread thread_down = std::thread(mergeSort, array, begin, mid);
        mergeSort(array, mid + 1, end);

        thread_down.join();
        mutex.lock(); active_threads -= 1; mutex.unlock();
    } else
    {
        mergeSort(array, begin, mid);
        mergeSort(array, mid + 1, end);
        merge(array, begin, mid, end);
    }
	
}

void printArray(int A[], int size)
{
	for (int i = 0; i < size; i++)
		std::cout << A[i] << " ";
}

int main()
{
    int arr[TABLE_SIZE] = {0};
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        arr[i] = std::rand() % 100;
    }
    
	int arr_size = TABLE_SIZE;


	std::cout << "Given array is \n";
	printArray(arr, arr_size);

	std::thread threads[NB_THREADS];

    mergeSort(arr, 0, arr_size - 1);
    
	std::cout << "\nSorted array is \n";
	printArray(arr, arr_size);
	return 0;
}

/*
$ g++ -std=c++17 others/merge.cpp && ./a.out
Given array is 
7 49 73 58 30 72 44 78 23 9 40 65 92 42 87 3 27 29 40 12 3 69 9 57 60 33 99 78 16 35 
Sorted array is 
7 9 23 30 40 42 44 49 58 65 72 73 78 87 92 3 3 9 12 16 27 29 33 35 40 57 60 69 78 99 
*/