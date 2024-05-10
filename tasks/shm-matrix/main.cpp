#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <fstream>
#include <cstring>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <omp.h>

#include "matrix.h"

struct Header {
    std::atomic_int status;
    int n;
    int matrix_size;
};

void multiply_matrices(uint32_t* a, uint32_t* b, uint32_t* result, int size) {
    Matrix A(a, size);
    Matrix B(b, size);
    Matrix Res(result, size);
    Matrix::ParallelMultiply(&A, &B, &Res);
}

// Выполняет бинарное возведение матрицы matrix в степень power
void matrix_exponentiation(uint32_t* matrix, int size, int power) {
    uint32_t* result = (uint32_t*)malloc(size * size * sizeof(uint32_t));
    uint32_t* temp = (uint32_t*)malloc(size * size * sizeof(uint32_t));

    // Инициализация единичной матрицы в result
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            result[i * size + j] = (i == j) ? 1 : 0;
        }
    }

    memcpy(temp, matrix, size * size * sizeof(uint32_t));

    // Бинарное возведение в степень
    while (power > 0) {
        if (power % 2 == 1) {
            multiply_matrices(result, temp, result, size);
        }
        multiply_matrices(temp, temp, temp, size);
        power /= 2;
    }

    memcpy(matrix + size * size, result, size * size * sizeof(uint32_t));

    free(result);
    free(temp);
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        return 1;
    }

    const char* shm_name = argv[1];

    const size_t shm_size = 64 * 1024 * 1024; // 64 MB
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        return 1;
    }

    if (ftruncate(shm_fd, shm_size) == -1) {
        return 1;
    }

    void* ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        return 1;
    }

    Header* header = reinterpret_cast<Header*>(ptr);

    while (header->status.load() != -1) {
        if (header->status.load() == 0) {
            header->status.store(1);
            int n = header->n;
            int matrix_size = header->matrix_size;
            uint32_t* matrix = reinterpret_cast<uint32_t*>(static_cast<char*>(ptr) + 12);

            matrix_exponentiation(matrix, matrix_size, n);

            header->status.store(2);
        }
    }

    munmap(ptr, shm_size);
    close(shm_fd);

    return 0;
}