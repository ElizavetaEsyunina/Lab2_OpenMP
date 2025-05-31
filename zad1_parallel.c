#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define MAX_VALUE 1000
#define NUM_RUNS 100

int main() {
    int array_size;
    int num_threads;
    char* array_size_str;
    char* num_threads_str;
    char* endptr;
    double total_time = 0.0;

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

    omp_set_num_threads(num_threads);
    // Запуск несколько раз
    for (int run = 0; run < NUM_RUNS; run++) {
        int* array = (int*)malloc(array_size * sizeof(int));
        if (array == NULL) {
            fprintf(stderr, "Ошибка выделения памяти\n");
            return 1;
        }

        srand(time(NULL) + run);
        for (int i = 0; i < array_size; i++) {
            array[i] = rand() % MAX_VALUE;
        }

        long long sum = 0;
        clock_t start = clock();

#pragma omp parallel for reduction(+:sum)
        for (int i = 0; i < array_size; i++) {
            sum += array[i];
        }

        clock_t end = clock();
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        total_time += time_spent;

        printf("Запуск %d:\n", run + 1);
        printf("Параллельная сумма: %lld\n", sum);
        printf("Время выполнения: %f секунд\n", time_spent);

        free(array);
        array = NULL;
    }

    double average_time = total_time / NUM_RUNS;
    printf("Среднее время выполнения: %f секунд\n", average_time);
    return 0;
}