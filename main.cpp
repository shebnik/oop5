#include <iostream>
#include <vector>
#include <pthread.h>
#include <ctime>
#include <chrono>

using namespace std;

// Function to multiply two matrices
void multiply_matrices(int **A, int **B, int **C, int size, int start_row, int end_row)
{
    for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < size; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < size; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Struct to pass arguments to the thread function
struct thread_data
{
    int **A;
    int **B;
    int **C;
    int size;
    int start_row;
    int end_row;
};

// Thread function
void *thread_function(void *arg)
{
    thread_data *data = static_cast<thread_data *>(arg);
    multiply_matrices(data->A, data->B, data->C, data->size, data->start_row, data->end_row);
    return NULL;
}

// Function to measure execution time
double measure_execution_time(int size, int num_threads)
{
    // Initialize matrices
    int **A = new int *[size];
    int **B = new int *[size];
    int **C = new int *[size];
    srand(static_cast<unsigned int>(time(NULL)));
    for (int i = 0; i < size; i++)
    {
        A[i] = new int[size];
        B[i] = new int[size];
        C[i] = new int[size];
    }

    // Populate matrices with random values
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    // Create threads and divide work
    pthread_t *threads = new pthread_t[num_threads];
    thread_data *data = new thread_data[num_threads];
    int chunk_size = size / num_threads;
    int start_row = 0;
    for (int i = 0; i < num_threads; i++)
    {
        data[i].A = A;
        data[i].B = B;
        data[i].C = C;
        data[i].size = size;
        data[i].start_row = start_row;
        data[i].end_row = start_row + chunk_size;
        if (i == num_threads - 1)
        {
            data[i].end_row = size;
        }
        pthread_create(&threads[i], NULL, thread_function, &data[i]);
        start_row += chunk_size;
    }

    // Start timer
    auto start_time = chrono::high_resolution_clock::now();

    // Wait for threads to finish
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Stop timer and calculate execution time
    auto end_time = chrono::high_resolution_clock::now();
    double execution_time = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count() / 1e6;

    // Free memory
    for (int i = 0; i < size; i++)
    {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;
    delete[] threads;
    delete[] data;

    return execution_time;
}

int main()
{
    int sizes[] = {100, 1000, 2000, 5000};
    int num_threads_values[] = {2, 4, 8, 16, 32};

    for (int size : sizes)
    {
        for (int num_thread : num_threads_values)
        {
            cout << "The size of the matrix: " << size << "x" << size << ", number of threads: " << num_thread << endl;
            double execution_time = measure_execution_time(size, num_thread);
            cout << "Execution time: " << execution_time << " s." << endl;
        }
        cout << endl;
    }

    return 0;
}