#include <iostream>
#include <thread>
#include <string>
#include "../include/lru_cache.hpp"

void print_separator() {
    std::cout << "----------------------------------------\n";
}

void demo_basic_usage() {
    std::cout << "\n=== Demo 1: Basic get/put ===\n";
    print_separator();

    LRUCache<std::string, int> cache(3);

    cache.put("a", 1);
    cache.put("b", 2);
    cache.put("c", 3);

    std::cout << "Inserted a=1, b=2, c=3 (capacity 3)\n";
    std::cout << "get(a) = " << cache.get("a").value() << "\n";

    cache.put("d", 4);
    std::cout << "Inserted d=4, capacity exceeded -- least recently used evicted\n";

    if (!cache.contains("b")) {
        std::cout << "b was evicted (it was least recently used after a was accessed)\n";
    } else {
        std::cout << "b is still present\n";
    }

    std::cout << "Cache size: " << cache.size() << "/" << cache.capacity() << "\n";
}

void demo_lru_ordering() {
    std::cout << "\n=== Demo 2: LRU ordering under pressure ===\n";
    print_separator();

    LRUCache<int, std::string> cache(2);

    cache.put(1, "one");
    cache.put(2, "two");
    std::cout << "Cache: {1: one, 2: two}\n";

    cache.get(1);
    std::cout << "Accessed key 1 -- it's now most recently used\n";

    cache.put(3, "three");
    std::cout << "Inserted key 3 -- key 2 should be evicted (least recently used)\n";

    std::cout << "contains(1): " << (cache.contains(1) ? "yes" : "no") << "\n";
    std::cout << "contains(2): " << (cache.contains(2) ? "yes" : "no") << "\n";
    std::cout << "contains(3): " << (cache.contains(3) ? "yes" : "no") << "\n";
}

void demo_ttl_expiry() {
    std::cout << "\n=== Demo 3: TTL expiry ===\n";
    print_separator();

    LRUCache<std::string, std::string> cache(5);

    cache.put_with_ttl("session", "user-data", std::chrono::seconds(1));
    std::cout << "Stored 'session' with 1 second TTL\n";

    auto immediate = cache.get("session");
    std::cout << "Immediate read: " << (immediate.has_value() ? immediate.value() : "MISSING") << "\n";

    std::cout << "Waiting 2 seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    auto after_expiry = cache.get("session");
    std::cout << "Read after expiry: " << (after_expiry.has_value() ? after_expiry.value() : "EXPIRED (correctly evicted)") << "\n";
}

void demo_stats() {
    std::cout << "\n=== Demo 4: Hit/miss statistics ===\n";
    print_separator();

    LRUCache<int, int> cache(3);

    cache.put(1, 100);
    cache.put(2, 200);

    cache.get(1);
    cache.get(2);
    cache.get(99);
    cache.get(98);
    cache.get(1);

    std::cout << "Hits: " << cache.hits() << "\n";
    std::cout << "Misses: " << cache.misses() << "\n";
    std::cout << "Hit rate: " << (cache.hit_rate() * 100) << "%\n";

    for (int i = 0; i < 10; i++) {
        cache.put(i, i * 10);
    }
    std::cout << "Evictions after inserting 10 items into capacity-3 cache: " << cache.evictions() << "\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "  LRU Cache\n";
    std::cout << "========================================\n";

    demo_basic_usage();
    demo_lru_ordering();
    demo_ttl_expiry();
    demo_stats();

    std::cout << "\n========================================\n";
    std::cout << "  All demos complete\n";
    std::cout << "========================================\n\n";

    return 0;
}