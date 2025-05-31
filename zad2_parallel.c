#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// Функция для обмена элементов
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Разделение массива
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Быстрая сортировка
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

#pragma omp task
        quickSort(arr, low, pi - 1);
#pragma omp task
        quickSort(arr, pi + 1, high);
    }
}

void ParallelQuickSort(int arr[], int n, int num_threads) {
    omp_set_num_threads(num_threads);

#pragma omp parallel
    {
#pragma omp single
        quickSort(arr, 0, n - 1);
    }
}
#define NUM_RUNS 100
#define MAX_VALUE 1000000
int main() {
    int array_size;
    int num_threads;
    char* array_size_str;
    char* num_threads_str;
    char* endptr;

    // Получение размера массива
    array_size_str = getenv("ARRAY_SIZE");
    if (array_size_str == NULL) {
        fprintf(stderr, "Переменная окружения ARRAY_SIZE не установлена\n");
        return 1;
    }

    array_size = strtol(array_size_str, &endptr, 10);
    if (*endptr != '\0' || array_size <= 0) {
        fprintf(stderr, "Некорректное значениe ARRAY_SIZE: %s\n", array_size_str);
        return 1;
    }

    // Получение количества потоков
    num_threads_str = getenv("NUM_THREADS");
    if (num_threads_str == NULL) {
        fprintf(stderr, "Переменная окружения NUM_THREADS не установлена\n");
        return 1;
    }

    num_threads = strtol(num_threads_str, &endptr, 10);
    if (*endptr != '\0' || num_threads <= 0) {
        fprintf(stderr, "Некорректное значение NUM_THREADS: %s\n", num_threads_str);
        return 1;
    }

    double total_time = 0.0;

    int* arr = (int*)malloc(array_size * sizeof(int));
    if (arr == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return 1;
    }
    for (int run = 0; run < NUM_RUNS; run++) {
        srand(time(NULL) + run);

        for (int i = 0; i < array_size; i++) {
            arr[i] = rand() % MAX_VALUE;
        }

        clock_t start = clock();
        ParallelQuickSort(arr, array_size, num_threads);
        clock_t end = clock();

        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Запуск %d:\n", run + 1);
        printf("Параллельная сортировка заняла %f секунд\n", time_spent);
        total_time += time_spent;
    }

    double average_time = total_time / NUM_RUNS;
    printf("Среднее время: %f секунд\n", average_time);

    free(arr);
    arr = NULL;

    return 0;
}