#pragma once

#include <numeric>
#include <cstdint>
#include <cstring>
#include <thread>


class Matrix {
public:
    uint32_t* m_data;
    int m_size;
    Matrix(uint32_t* data, int size) : m_data(data), m_size(size){};
    void SetValue(int i, int j, int value){
        m_data[i * m_size + j] = value;
    }
    void SetAll(int value){
        memset(m_data, m_size * m_size * 4, value);
    }

    static void Multiply(Matrix* a, Matrix* b, Matrix* res){
        uint32_t* temp = (uint32_t*)malloc(res->m_size * res->m_size * sizeof(uint32_t));
        memset(temp, 0, res->m_size * res->m_size * sizeof(uint32_t));

        // Транспонируем матрицу b
        uint32_t* transposed_b = (uint32_t*)malloc(res->m_size * res->m_size * sizeof(uint32_t));
        for (int i = 0; i < res->m_size; ++i) {
            for (int j = 0; j < res->m_size; ++j) {
                transposed_b[i * res->m_size + j] = b->m_data[j * res->m_size + i];
            }
        }

        for (int i = 0; i < res->m_size; ++i) {
            for (int j = 0; j < res->m_size; ++j) {
                uint32_t tempRes = 0;
                for (int k = 0; k < res->m_size; ++k) {
                    tempRes += a->m_data[i * res->m_size + k] * transposed_b[j * res->m_size + k];
                }
                temp[i * res->m_size + j] = tempRes;
            }
        }

        memcpy(res->m_data, temp, res->m_size * res->m_size * sizeof(uint32_t));

        // Освобождаем память
        free(temp);
        free(transposed_b);
    }

    static void Transpose(Matrix* a, Matrix* res){
        int r = res->m_size * res->m_size;
        int resultCol = 0;
        int resultRow = 0;

        int inputCol = 0;
        int inputRow = 0;

        for (int i = 0; i < r; ++i){
            resultRow = i / res->m_size;
            resultCol = i % res->m_size;

            inputRow = resultCol;
            inputCol = resultRow;

            res->m_data[i] = a->m_data[inputRow * res->m_size + inputCol];
        }
    }

    static void ParallelMultiply(Matrix* a, Matrix* b, Matrix* res) {
        uint32_t *temp = (uint32_t *) malloc(res->m_size * res->m_size * sizeof(uint32_t));
        memset(temp, 0, res->m_size * res->m_size * sizeof(uint32_t));

        Matrix Temp(temp, res->m_size);
        struct process_data_chunk {
            void operator()(Matrix *a, Matrix *b, Matrix *res, int startIndex, int endIndex) {
                for (int i = startIndex; i < endIndex; ++i) {
                    uint32_t sum = 0;
                    int firstTemp = (i / res->m_size) * res->m_size;
                    int secondTemp = i % res->m_size;
                    for (int j = 0; j < res->m_size; ++j) {
                        sum += a->m_data[firstTemp + j] * b->m_data[secondTemp * res->m_size + j];
                    }
                    res->m_data[i] = sum;
                }
            }
        };
        int length = res->m_size * res->m_size;
        if (!length)
            return;

        int min_pre_thread = 10000;
        int max_threads = (length + min_pre_thread - 1) / min_pre_thread;
        int hardware_threads = std::thread::hardware_concurrency();
        int num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
        int block_size = length / num_threads;

        std::vector<std::thread> threads(num_threads - 1);
        int block_start = 0;
        int block_end = 0;

        // Транспонируем матрицу b
        uint32_t *transposed_b = (uint32_t *) malloc(res->m_size * res->m_size * sizeof(uint32_t));
        for (int i = 0; i < res->m_size; ++i) {
            for (int j = 0; j < res->m_size; ++j) {
                transposed_b[i * res->m_size + j] = b->m_data[j * res->m_size + i];
            }
        }

        Matrix Transposed(transposed_b, res->m_size);

        for (int i = 0; i < num_threads - 1; ++i) {
            block_start = i * block_size;
            block_end = block_start + block_size;
            threads[i] = std::thread(process_data_chunk(), a, &Transposed, &Temp, block_start, block_end);
        }

        process_data_chunk()(a, &Transposed, &Temp, block_end, length);

        for (auto &thread: threads) {
            thread.join();
        }

        memcpy(res->m_data, Temp.m_data, res->m_size * res->m_size * sizeof(uint32_t));
        free(Temp.m_data);
    }
};
