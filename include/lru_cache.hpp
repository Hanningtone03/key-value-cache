#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP

#include <unordered_map>
#include <chrono>
#include <optional>
#include <stdexcept>
#include <cstddef>

template <typename K, typename V>
class LRUCache {
private:
    struct Node {
        K key;
        V value;
        std::chrono::steady_clock::time_point expiry;
        bool has_ttl;
        Node* prev;
        Node* next;

        Node(const K& k, const V& v)
            : key(k), value(v), has_ttl(false), prev(nullptr), next(nullptr) {}
    };

    size_t capacity_;
    std::unordered_map<K, Node*> map_;
    Node* head_;
    Node* tail_;

    size_t hits_;
    size_t misses_;
    size_t evictions_;

    void detach(Node* node) {
        if (node->prev) node->prev->next = node->next;
        else head_ = node->next;

        if (node->next) node->next->prev = node->prev;
        else tail_ = node->prev;

        node->prev = nullptr;
        node->next = nullptr;
    }

    void attach_to_front(Node* node) {
        node->next = head_;
        node->prev = nullptr;
        if (head_) head_->prev = node;
        head_ = node;
        if (!tail_) tail_ = node;
    }

    void move_to_front(Node* node) {
        if (node == head_) return;
        detach(node);
        attach_to_front(node);
    }

    bool is_expired(Node* node) const {
        if (!node->has_ttl) return false;
        return std::chrono::steady_clock::now() > node->expiry;
    }

    void evict_tail() {
        if (!tail_) return;
        Node* victim = tail_;
        detach(victim);
        map_.erase(victim->key);
        delete victim;
        evictions_++;
    }

public:
    explicit LRUCache(size_t capacity)
        : capacity_(capacity), head_(nullptr), tail_(nullptr),
          hits_(0), misses_(0), evictions_(0) {
        if (capacity_ == 0) {
            throw std::invalid_argument("Cache capacity must be greater than zero");
        }
    }

    ~LRUCache() {
        clear();
    }

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

    std::optional<V> get(const K& key) {
        auto it = map_.find(key);
        if (it == map_.end()) {
            misses_++;
            return std::nullopt;
        }

        Node* node = it->second;

        if (is_expired(node)) {
            detach(node);
            map_.erase(it);
            delete node;
            misses_++;
            return std::nullopt;
        }

        move_to_front(node);
        hits_++;
        return node->value;
    }

    void put(const K& key, const V& value) {
        auto it = map_.find(key);

        if (it != map_.end()) {
            Node* node = it->second;
            node->value = value;
            node->has_ttl = false;
            move_to_front(node);
            return;
        }

        if (map_.size() >= capacity_) {
            evict_tail();
        }

        Node* node = new Node(key, value);
        attach_to_front(node);
        map_[key] = node;
    }

    template <typename Duration>
    void put_with_ttl(const K& key, const V& value, Duration ttl) {
        put(key, value);
        Node* node = map_[key];
        node->has_ttl = true;
        node->expiry = std::chrono::steady_clock::now() + ttl;
    }

    bool remove(const K& key) {
        auto it = map_.find(key);
        if (it == map_.end()) return false;

        Node* node = it->second;
        detach(node);
        map_.erase(it);
        delete node;
        return true;
    }

    bool contains(const K& key) const {
        return map_.find(key) != map_.end();
    }

    size_t size() const {
        return map_.size();
    }

    size_t capacity() const {
        return capacity_;
    }

    void clear() {
        Node* current = head_;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
        map_.clear();
    }

    size_t hits() const { return hits_; }
    size_t misses() const { return misses_; }
    size_t evictions() const { return evictions_; }

    double hit_rate() const {
        size_t total = hits_ + misses_;
        if (total == 0) return 0.0;
        return static_cast<double>(hits_) / static_cast<double>(total);
    }

    void reset_stats() {
        hits_ = 0;
        misses_ = 0;
        evictions_ = 0;
    }
};

#endif
