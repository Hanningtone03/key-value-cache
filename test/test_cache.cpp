#include <iostream>
#include <thread>
#include <string>
#include <cassert>
#include "../include/lru_cache.hpp"

int tests_run = 0;
int tests_passed = 0;

#define CHECK(condition, description) \
    do { \
        tests_run++; \
        if (condition) { \
            tests_passed++; \
            std::cout << "  [PASS] " << description << "\n"; \
        } else { \
            std::cout << "  [FAIL] " << description << "\n"; \
        } \
    } while (0)

void test_basic_get_put() {
    std::cout << "\nTest group: basic get/put\n";
    LRUCache<std::string, int> cache(3);

    cache.put("a", 1);
    CHECK(cache.get("a").has_value() && cache.get("a").value() == 1, "get returns the value just put");
    CHECK(!cache.get("nonexistent").has_value(), "get on missing key returns nullopt");
    CHECK(cache.size() == 1, "size reflects one entry");
}

void test_eviction_order() {
    std::cout << "\nTest group: eviction order\n";
    LRUCache<int, int> cache(2);

    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(3, 30);

    CHECK(!cache.contains(1), "oldest entry evicted when capacity exceeded");
    CHECK(cache.contains(2), "second entry survives");
    CHECK(cache.contains(3), "newest entry survives");
}

void test_access_updates_recency() {
    std::cout << "\nTest group: access updates recency\n";
    LRUCache<int, int> cache(2);

    cache.put(1, 10);
    cache.put(2, 20);
    cache.get(1);
    cache.put(3, 30);

    CHECK(cache.contains(1), "recently accessed entry survives eviction");
    CHECK(!cache.contains(2), "least recently used entry evicted instead");
}

void test_update_existing_key() {
    std::cout << "\nTest group: updating an existing key\n";
    LRUCache<std::string, int> cache(3);

    cache.put("x", 1);
    cache.put("x", 2);

    CHECK(cache.get("x").value() == 2, "updating a key overwrites its value");
    CHECK(cache.size() == 1, "updating a key does not increase size");
}

void test_remove() {
    std::cout << "\nTest group: remove\n";
    LRUCache<int, int> cache(3);

    cache.put(1, 10);
    bool removed = cache.remove(1);
    bool removed_again = cache.remove(1);

    CHECK(removed, "remove returns true for an existing key");
    CHECK(!removed_again, "remove returns false for an already-removed key");
    CHECK(!cache.contains(1), "removed key is no longer present");
}

void test_ttl_expiry() {
    std::cout << "\nTest group: TTL expiry\n";
    LRUCache<std::string, std::string> cache(3);

    cache.put_with_ttl("temp", "value", std::chrono::milliseconds(50));
    CHECK(cache.get("temp").has_value(), "value is present immediately after insert");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    CHECK(!cache.get("temp").has_value(), "value is gone after TTL expires");
}

void test_capacity_one() {
    std::cout << "\nTest group: capacity of one\n";
    LRUCache<int, int> cache(1);

    cache.put(1, 100);
    cache.put(2, 200);

    CHECK(!cache.contains(1), "first entry evicted immediately with capacity 1");
    CHECK(cache.contains(2), "only the most recent entry survives");
}

void test_invalid_capacity() {
    std::cout << "\nTest group: invalid capacity\n";
    bool threw = false;
    try {
        LRUCache<int, int> cache(0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    CHECK(threw, "constructing with capacity zero throws");
}

void test_stats_tracking() {
    std::cout << "\nTest group: stats tracking\n";
    LRUCache<int, int> cache(2);

    cache.put(1, 10);
    cache.get(1);
    cache.get(2);

    CHECK(cache.hits() == 1, "hit count is tracked correctly");
    CHECK(cache.misses() == 1, "miss count is tracked correctly");
}

void test_clear() {
    std::cout << "\nTest group: clear\n";
    LRUCache<int, int> cache(3);

    cache.put(1, 10);
    cache.put(2, 20);
    cache.clear();

    CHECK(cache.size() == 0, "clear empties the cache");
    CHECK(!cache.contains(1), "cleared entries are no longer accessible");
}

int main() {
    std::cout << "========================================\n";
    std::cout << "  LRU Cache Test Suite\n";
    std::cout << "========================================\n";

    test_basic_get_put();
    test_eviction_order();
    test_access_updates_recency();
    test_update_existing_key();
    test_remove();
    test_ttl_expiry();
    test_capacity_one();
    test_invalid_capacity();
    test_stats_tracking();
    test_clear();

    std::cout << "\n========================================\n";
    std::cout << "  " << tests_passed << "/" << tests_run << " tests passed\n";
    std::cout << "========================================\n\n";

    return (tests_passed == tests_run) ? 0 : 1;
}