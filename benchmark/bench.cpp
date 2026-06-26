#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include "../include/lru_cache.hpp"

double benchmark_puts(size_t cache_size, size_t num_operations) {
    LRUCache<int, int> cache(cache_size);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, static_cast<int>(cache_size * 2));

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_operations; i++) {
        cache.put(dist(rng), static_cast<int>(i));
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

double benchmark_gets(size_t cache_size, size_t num_operations) {
    LRUCache<int, int> cache(cache_size);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, static_cast<int>(cache_size * 2));

    for (size_t i = 0; i < cache_size; i++) {
        cache.put(static_cast<int>(i), static_cast<int>(i));
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_operations; i++) {
        cache.get(dist(rng));
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

double benchmark_mixed_workload(size_t cache_size, size_t num_operations) {
    LRUCache<int, int> cache(cache_size);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> key_dist(0, static_cast<int>(cache_size * 3));
    std::uniform_int_distribution<int> op_dist(0, 1);

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_operations; i++) {
        int key = key_dist(rng);
        if (op_dist(rng) == 0) {
            cache.put(key, static_cast<int>(i));
        } else {
            cache.get(key);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

void print_result(const std::string& label, size_t cache_size, size_t ops, double ms) {
    double ops_per_sec = (static_cast<double>(ops) / ms) * 1000.0;
    std::cout << "  " << label << "\n";
    std::cout << "    Cache size: " << cache_size << ", Operations: " << ops << "\n";
    std::cout << "    Time: " << ms << " ms\n";
    std::cout << "    Throughput: " << static_cast<long>(ops_per_sec) << " ops/sec\n\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "  LRU Cache Benchmark\n";
    std::cout << "========================================\n\n";

    const size_t cache_size = 1000;
    const size_t num_operations = 1000000;

    double put_time = benchmark_puts(cache_size, num_operations);
    print_result("PUT operations", cache_size, num_operations, put_time);

    double get_time = benchmark_gets(cache_size, num_operations);
    print_result("GET operations", cache_size, num_operations, get_time);

    double mixed_time = benchmark_mixed_workload(cache_size, num_operations);
    print_result("Mixed GET/PUT workload", cache_size, num_operations, mixed_time);

    std::cout << "========================================\n";
    std::cout << "  Benchmark complete\n";
    std::cout << "========================================\n\n";

    return 0;
}