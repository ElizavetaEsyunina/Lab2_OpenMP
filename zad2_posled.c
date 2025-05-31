#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//функция для обмена элементов
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

//разделение массива и возврат индекса опорного элемента
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

//быстрая сортировка
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

#define MAX_VALUE 100000
#define NUM_RUNS 100

int main() {
    int array_size;
    char* array_size_str = getenv("ARRAY_SIZE");
    if (array_size_str == NULL) {
        fprintf(stderr, "Переменная окружения ARRAY_SIZE не установлена\n");
        return 1;
    }

    char* endptr;
    array_size = strtol(array_size_str, &endptr, 10);

    if (*endptr != '\0' || array_size <= 0) {
        fprintf(stderr, "Некорректное значение ARRAY_SIZE: %s\n", array_size_str);
        return 1;
    }

    double total_time = 0.0;

    for (int run = 0; run < NUM_RUNS; run++) {
        int* arr = (int*)malloc(array_size * sizeof(int));
        if (arr == NULL) {
            fprintf(stderr, "Ошибка выделения памяти\n");
            return 1;
        }

        srand(time(NULL) + run);

        for (int i = 0; i < array_size; i++) {
            arr[i] = rand() % MAX_VALUE;
        }

        clock_t start = clock();
        quickSort(arr, 0, array_size - 1);
        clock_t end = clock();

        double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC;
        total_time += time_spent;

        //проверка, что массив отсортирован
        for (int i = 0; i < array_size - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                printf("Массив не отсортирован!\n");
                break;
            }
        }

        printf("Запуск %d:\n", run + 1);
        printf("Последовательная сортировка заняла %f секунд\n", time_spent);

        free(arr);
        arr = NULL;
    }

    double average_time = total_time / NUM_RUNS;
    printf("Среднее время выполнения: %f секунд\n", average_time);
    return 0;
}