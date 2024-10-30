#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_RUNS 10
#define MAX_THREADS 16
#define SIZE 10000000

void generate_array(int *array, int count, int seed, int threads);

int sedgewick(int* gap_arr, int size){          // << вычисление массива Седжвика
    int multiplier1 = 1, multiplier2 = 1, multiplier3 = 1, counter = -1;
    do{
        if(++counter%2){  // << нечетный проход
            multiplier2 *= 2;
            gap_arr[counter] = 8*multiplier1 - 6*multiplier2 + 1;
        }else{            // << четный проход
            gap_arr[counter] = 9*multiplier1 - 9*multiplier3 + 1;
            multiplier3 *= 2;
        }
        multiplier1 *= 2;
    }while(3*gap_arr[counter] < size);
    return((counter>0)?(--counter):(0));
}

double shell_sort(int *array, int size, int threads) {
    double start, end;
    start = omp_get_wtime();
    int gap, j, counter;
    int gap_arr[50];
    counter = sedgewick(gap_arr, size);
    while(counter >= 0) {
        gap = gap_arr[counter--];
#pragma omp parallel for num_threads(threads) shared(array, size, gap) private(j) default(none)
        for( int i = 0; i < gap; i++)
        {
            for(j = gap + i; j < size; j += gap) {
                int k = j;
                while (k > i && array[k-gap] > array[k]) {
                    int temp = array[k];
                    array[k] = array[k-gap];
                    array[k-=gap] = temp;
                }
            }
        }
    }
    end = omp_get_wtime();
    printf("Threads: %d, Time: %f, First element: %d\n", threads, end - start, array[0]);
    return (end - start);
}

int main() {
    FILE *file = fopen("results_ft3.txt", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }
    printf("\tACCURACY == %f\n", omp_get_wtick());
    int *arrays[NUM_RUNS];
    for (int threads = 1; threads <= MAX_THREADS; threads++) {
        double time_spent = 0;
        for (int i = 0; i < NUM_RUNS; i++) {
            arrays[i] = (int *)malloc(SIZE * sizeof(int));
            generate_array(arrays[i], SIZE, i + 920215, threads);
        }
        for (int run = 0; run < NUM_RUNS; run++) {
            time_spent += shell_sort(arrays[run], SIZE, threads);
        }
        time_spent = time_spent/NUM_RUNS;
        for (int i = 0; i < NUM_RUNS; i++) {
            free(arrays[i]);
        }
        fprintf(file, "%d %f\n", threads, time_spent);
    }
    fclose(file);
    return 0;
}

void generate_array(int *array, int count, int seed, int threads) {
    srand(seed);
#pragma omp parallel for num_threads(threads)   // << параллельная инициализация массива для эффекта первого касания
    for (int i = 0; i < count; i++) {
        array[i] = (rand()+1)*rand();
    }
}
