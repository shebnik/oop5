#include <iostream>
#include <vector>
#include <ctime>
#include <pthread.h>

using namespace std;

// Структура для передачі аргументів у потік
struct thread_args {
    vector<vector<double>> *A;
    vector<double> *b;
    int start_row, end_row;
};

// Функція, яку виконуватимуть потоки
void *gaussian_elimination(void *args) {
    thread_args *targs = static_cast<thread_args *>(args);
    vector<vector<double>> &A = *targs->A;
    vector<double> &b = *targs->b;
    int start_row = targs->start_row;
    int end_row = targs->end_row;
    int n = A.size();

    for (int k = 0; k < n; k++) {
        for (int i = start_row; i < end_row; i++) {
            if (i != k) {
                double factor = A[i][k] / A[k][k];
                for (int j = k; j < n; j++) {
                    A[i][j] -= factor * A[k][j];
                }
                b[i] -= factor * b[k];
            }
        }
    }

    return nullptr;
}

double measure_execution_time(int size, int num_threads) {
    vector<vector<double>> A(size, vector<double>(size));
    vector<double> b(size);

    // Ініціалізація матриці A та вектора b
    srand(time(nullptr));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            A[i][j] = rand() % 10;
        }
        b[i] = rand() % 10;
    }

    // Створення потоків    
    pthread_t *threads = new pthread_t[num_threads];
    thread_args *args = new thread_args[num_threads];
    int chunk_size = size / num_threads;

    clock_t start_time = clock();

    for (int i = 0; i < num_threads; i++) {
        args[i].A = &A;
        args[i].b = &b;
        args[i].start_row = i * chunk_size;
        args[i].end_row = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
        pthread_create(&threads[i], nullptr, gaussian_elimination, &args[i]);
    }

    // Очікування завершення потоків
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], nullptr);
    }

    clock_t end_time = clock();
    double execution_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;

    return execution_time;
}

int main() {
    int sizes[] = {100, 1000, 2000, 5000};
    int num_threads_values[] = {2, 4, 8, 16, 32};

    for (int size : sizes) {
        for (int num_thread : num_threads_values) {
            cout << "The size of the matrix: " << size << "x" << size << ", number of threads: " << num_thread << endl;
            double execution_time = measure_execution_time(size, num_thread);
            cout << "Execution time: " << execution_time << " s." << endl;
        }
        cout << endl;
    }

    return 0;
}