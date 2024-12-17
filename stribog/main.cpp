#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include "gost341118.h"
#include <chrono>

std::vector<unsigned char> generate_random_message(size_t size) {
    std::vector<unsigned char> message(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255);

    for (size_t i = 0; i < size; ++i) {
        message[i] = dis(gen);
    }
    return message;
}

// Функция для замера времени выполнения (последовательная версия)
double benchmark(Streebog &hash, const std::vector<unsigned char>& message) {
    auto start = omp_get_wtime();
    hash.Hash(const_cast<unsigned char*>(message.data()), message.size());
    auto end = omp_get_wtime();
    double duration = end - start;
    return duration;
}

int main() {
    // Количество тестов
    const int num_tests = 10;
    const size_t message_size = 10 * 1024 * 1024;  // 10 MB

    Streebog hash_512(512); // Стрибог в режиме 512
    Streebog hash_256(256); // Стрибог в режиме 256

    // Генерация случайных сообщений для тестирования
    std::vector<std::vector<unsigned char>> messages(num_tests);
    for (int i = 0; i < num_tests; ++i) {
        messages[i] = generate_random_message(message_size);
    }

    // Замер времени для последовательной версии алгоритма
    double total_time = 0.0;
    for (int i = 0; i < num_tests; ++i) {
        total_time += benchmark(hash_512, messages[i]);
    }
    double avg_time = total_time / num_tests;
    std::cout << "Average sequential time: " << avg_time << " seconds." << std::endl;



    return 0;
}

