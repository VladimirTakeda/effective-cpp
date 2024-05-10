#include <iostream>
#include <algorithm>
#include <numeric>

#include <chrono>
#include <vector>
#include <string>
#include <string_view>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <random>

void FirstApproach(){
    int *array = new int [32 * 1024 * 1024];
    for (int step = 1; step < 1000; step *= 2){
        int i = 0;
        auto start_time = std::chrono::high_resolution_clock::now();
        for (int repeat = 0; repeat < 10000; ++repeat){
            array[i]++;
            i = (i + step) % 33554432;
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end_time - start_time;
        std::cout << "Time : " << elapsed_seconds.count() << " step : " << step << "\n";
    }
}

void SecondApproach(){
    int *array = new int [128 * 1024 * 1024];
    for (int step = 1000000; step < 200 * 1000000; step *= 2){
        auto start_time = std::chrono::high_resolution_clock::now();
        for (int times = 0; times < 10000; times++) {
            for (int index = 0; index < step; ++index) {
                ++array[index];
            }
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end_time - start_time;
        std::cout << "Time without for the loop: " << elapsed_seconds.count() << "; size : " << step << " bytes" << "\n";
    }
}

void LastApproach(){
    const int buffer_size = 1000000;
    char buffer[buffer_size];
    std::ostringstream output(buffer, std::ios_base::ate);
    output << std::fixed;
    const size_t limit = 128 * 1024 * 1024;
    std::unique_ptr<int []> ptr(new int[limit]);
    std::string s;
    for (size_t i = 10 * 1024; i < 200 * 1024; i += 10 * 1024){
        auto start_time = std::chrono::high_resolution_clock::now();
        std::iota(ptr.get(), ptr.get() + i, 0);
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end_time - start_time;
        output << "Time without for the loop: " << elapsed_seconds.count() << "\n";
    }
    std::cout << output.str();
}

void MyApproach(){
    const int buffer_size = 1000000;
    char buffer[buffer_size];
    std::ostringstream output(buffer, std::ios_base::ate);
    output << std::fixed;
    for (size_t i = 1024; i < 64 * 1024 * 1024; i*=2) {
        auto start_time = std::chrono::high_resolution_clock::now();
        const size_t steps = 64 * 1024 * 1024;
        int lenghtMethod = i - 1;
        std::unique_ptr<int[]> ptr(new int[i]);
        for (int j = 0; j < steps; ++j) {
            ptr[(j * 16) & lenghtMethod]++;
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end_time - start_time;
        output << "Time without for the loop: " << elapsed_seconds.count() << " size: " << i << "\n";
    }
    std::cout << output.str();
}

void MyApproach1(){
    for (size_t currSize = 1024 / sizeof (int); currSize < 64 * 1024 * 1024 / sizeof (int); currSize *=2) {
        const size_t steps = 64 * 1024 * 1024;
        int lenghtMethod = currSize - 1;
        std::unique_ptr<int[]> ptr(new int[currSize]);
        auto start_time = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < steps; ++j) {
            ptr[(j * 16) & lenghtMethod]++;
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end_time - start_time;
        std::cout << "Time without for the loop: " << elapsed_seconds.count() << " size: " << currSize * sizeof (int) << "\n";
    }
}

void SergeysApproach(){
    std::random_device rd;
    std::mt19937 g(rd());
    for (size_t currSize = 1024 / sizeof (int); currSize < 64 * 1024 * 1024 / sizeof (int); currSize *=2) {
        std::unique_ptr<int[]> mainArray(new int[currSize]);
        std::unique_ptr<int[]> indexArray(new int[currSize]);
        std::iota(indexArray.get(), indexArray.get() + currSize, 0);
        std::shuffle(indexArray.get(), indexArray.get() + currSize, g);
        auto start_time = std::chrono::high_resolution_clock::now();

        int mainIndex = 0;
        for (int index = 0; index < currSize; ++index){
            mainArray[mainIndex] = indexArray[index];
            mainIndex = indexArray[index];
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end_time - start_time;
        std::cout << "Time without for the loop: " << elapsed_seconds.count() << " size: " << currSize << "\n";
    }
}

int main(){
    std::cout << std::fixed;
    MyApproach1();
}